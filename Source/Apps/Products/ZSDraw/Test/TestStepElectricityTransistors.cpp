/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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
#include "ZSDrawGraphObjElectricityTransistor.h"

#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSDraw/Widgets/Drawing/ZSDrawingView.h"
#include "ZSDraw/Drawing/ObjFactories/ZSDrawObjFactory.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::Draw::Electricity;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

const int     c_iGraphObjType        = EGraphObjTypeUserDefined;
const QString c_strGraphObjNameSpace = "ZS::Draw::Electricity";
const QString c_strGraphObjClassName = "CGraphObjTransistor";
const QString c_strDrawingTool       = "Electricity::Transistor";

static QRectF s_rctTransistor1;

// Create_Transistor1
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosCreateTransistor1(650,300);
static const QPoint s_ptDrawingSceneMousePosCreateTransistor1Finished(630,280);

// Resize_Transistor1_MovePtBottomRight
//------------------------------------------------------------------------------

static const QPoint s_ptDrawingSceneMousePosResizeTransistor1Select(671,324);

static const QPoint s_ptDrawingSceneMousePosResizeTransistor1PtBottomRightStart(692,348); // CGraphObjTransistor::c_sizInitial(42.0,48.0)

static const QSize s_arsizeGroupsResizeTransistor1MovePtBottomRightTestSteps[] = {
                    /* Transistor1:      l,      t,      r,      b */
                               /* 650.00, 250.00, 692.00, 258.00 */
    /*  0 */ QSize(  21,  4 ), /* 650.00, 250.00, 713.00, 262.00 */
    /*  1 */ QSize(  21,  4 ), /* 650.00, 250.00, 734.00, 266.00 */
    /*  2 */ QSize( -21, -4 ), /* 650.00, 250.00, 713.00, 262.00 */
    /*  3 */ QSize( -21, -4 ), /* 650.00, 250.00, 692.00, 258.00 */
    /*  4 */ QSize( -21, -4 ), /* 650.00, 250.00, 671.00, 254.00 */
    /*  5 */ QSize( -21, -4 ), /* 650.00, 250.00, 650.00, 250.00 */
    /*  6 */ QSize( -21, -4 ), /* 629.00, 246.00, 650.00, 250.00 */
    /*  7 */ QSize( -21, -4 ), /* 608.00, 242.00, 650.00, 250.00 */
    /*  8 */ QSize(  21,  4 ), /* 629.00, 246.00, 650.00, 250.00 */
    /*  9 */ QSize(  21,  4 ), /* 650.00, 250.00, 650.00, 250.00 */
    /*  8 */ QSize(  21,  4 ), /* 650.00, 250.00, 671.00, 254.00 */
    /*  9 */ QSize(  21,  4 ), /* 650.00, 250.00, 692.00, 258.00 */
    /* 10 */ QSize(  21, 24 )  /* 650.00, 250.00, 692.00, 258.00 */
};

static const int ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepCount = sizeof(s_arsizeGroupsResizeTransistor1MovePtBottomRightTestSteps)/sizeof(s_arsizeGroupsResizeTransistor1MovePtBottomRightTestSteps[0]);

// Test steps:
//------------------------------------------------------------------------------

enum
{
    ETestStepLev3_Transistors_Create_Transistor1_ActivateEditModeSelect,
    ETestStepLev3_Transistors_Create_Transistor1_ActivateDrawingTool,
    ETestStepLev3_Transistors_Create_Transistor1,
    ETestStepLev3_Transistors_Create_Transistor1_Finished,
    ETestStepLev3_Transistors_Resize_Transistor1_ActivateEditModeSelect,
    ETestStepLev3_Transistors_Resize_Transistor1_Select,
    ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Select,
    ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMin,
    ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMax = ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMin + ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepCount - 1,
    ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Release,
    ETestStepLev3_Transistors_Resize_Transistor1_Finished,
    ETestStepLev3_Transistors_Finished,
    ETestStepLev3_Transistors_Count
};

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepElectricityTransistors( ZS::Test::CTestStepGroup* i_pTestStepGrpParent )
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

    ZS::Test::CTestStepGroup* pTSGrpTransistors       = getTestStepGroup( "Transistors", i_pTestStepGrpParent );
    ZS::Test::CTestStepGroup* pTSGrpTransistorsCreate = getTestStepGroup( "Create", pTSGrpTransistors );
    ZS::Test::CTestStepGroup* pTSGrpTransistorsResize = getTestStepGroup( "Resize", pTSGrpTransistors );

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
    else if( !pTSGrpTransistors->isEnabled() )
    {
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else if( !pTSGrpTransistorsCreate->isEnabled() )
    {
        // If the transistor is not created none of the other sub tests may be performed.
        m_iTestStepLev2++; // Next test step
        m_iTestStepLev3 = 0;
        triggerDoTestStep();
    }
    else // if( pTSGrpTransistorsCreate->isEnabled() )
    {
        if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_ActivateEditModeSelect )
        {
            pTestStep = getTestStep( "Create.T1.ActivateEditModeSelect", pTSGrpTransistorsCreate );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_ActivateDrawingTool )
        {
            pTestStep = getTestStep( "Create.T1.ActivateDrawingTool", pTSGrpTransistorsCreate );

            strlstExpectedValues.append( "EditTool: Select" );
            strlstExpectedValues.append( "DrawingTool: " );

            pTestStep->setOperation( "ToolBoxTreeView.selectObjFactoryNode( Electricity/Transistor )" );
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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_ActivateDrawingTool )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1 )
        {
            m_iElectricityTransistorsCount++;

            m_iTestStepLev4 = -1;

            strCurrentDrawingTool = c_strDrawingTool;

            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj, pTSGrpTransistorsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateTransistor1;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctTransistor1 = QRectF( m_ptDrawingSceneMousePos, CGraphObjTransistor::c_sizInitial );

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctTransistor1) );

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

                m_pDrawingScene->onGraphObjCreationFinished(pGraphObj);

                m_pDrawingScene->setMode( EMode::Ignore, EEditTool::Select, EEditMode::Move, EEditResizeMode::Undefined, false );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1 )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_Finished )
        {
            strCurrentDrawingTool = c_strDrawingTool;

            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Create." + strCurrentGraphObj + ".Finished", pTSGrpTransistorsCreate );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosCreateTransistor1Finished;

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

            pTSGrpTransistorsCreate->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Create_Transistor1_Finished )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_ActivateEditModeSelect )
        {
            if( !pTSGrpTransistorsResize->isEnabled() )
            {
                m_iTestStepLev3 = ETestStepLev3_Transistors_Resize_Transistor1_Finished+1; // Last move test step
                triggerDoTestStep();
            }
            else // if( pTSGrpTransistorsResize->isEnabled() )
            {
                strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

                pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".ActivateEditModeSelect", pTSGrpTransistorsResize );

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

            } // if( pTSGrpTransistorsResize->isEnabled() )

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_ActivateEditModeSelect )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".Select", pTSGrpTransistorsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeTransistor1Select;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPoint::Undefined) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctTransistor1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_Select )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Select )
        {
            m_iTestStepLev4 = -1;

            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Select", pTSGrpTransistorsResize );

            m_ptDrawingSceneMousePos = s_ptDrawingSceneMousePosResizeTransistor1PtBottomRightStart;

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            strlstExpectedValues.append( strCurrentGraphObj + ": SelPt:" + selectionPoint2Str(ESelectionPointBottomRight) );
            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctTransistor1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Select )

        else if( m_iTestStepLev3 >= ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMax )
        {
            m_iTestStepLev4++;

            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Step" + QString::number(m_iTestStepLev4), pTSGrpTransistorsResize );

            m_fxOffsHor = s_arsizeGroupsResizeTransistor1MovePtBottomRightTestSteps[m_iTestStepLev4].width();
            m_fyOffsHor = s_arsizeGroupsResizeTransistor1MovePtBottomRightTestSteps[m_iTestStepLev4].height();

            m_ptDrawingSceneMousePos.setX( m_ptDrawingSceneMousePos.x() + Math::round2Nearest(m_fxOffsHor,0) );
            m_ptDrawingSceneMousePos.setY( m_ptDrawingSceneMousePos.y() + Math::round2Nearest(m_fyOffsHor,0) );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            s_rctTransistor1.setRight(m_ptDrawingSceneMousePos.x());
            s_rctTransistor1.setBottom(m_ptDrawingSceneMousePos.y());

            // Keep old size before resizing starts as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            QRectF rct = s_rctTransistor1;

            if( fabs(s_rctTransistor1.width()) > 0.0 || fabs(s_rctTransistor1.height()) > 0.0 )
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

        } // if( m_iTestStepLev3 >= ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMin && m_iTestStepLev3 <= ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_TestStepMax )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Release )
        {
            strCurrentGraphObj = "T" + QString::number(m_iElectricityTransistorsCount);

            pTestStep = getTestStep( "Resize." + strCurrentGraphObj + ".MovePtBottomRight.Release", pTSGrpTransistorsResize );

            m_ptDrawingViewMousePos = m_pDrawingView->mapFromScene(m_ptDrawingSceneMousePos);

            ptMousePosGlobal = m_pDrawingView->mapToGlobal(m_ptDrawingViewMousePos);
            ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
            ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

            // Old size before resizing starts has been kept as the object may get 0 pixel size.
            // The size will be adjusted on releasing the mouse.
            if( s_rctTransistor1.width() < 0 )
            {
                double fLeft = s_rctTransistor1.left();
                double fRight = s_rctTransistor1.right();
                s_rctTransistor1.setLeft(fRight);
                s_rctTransistor1.setRight(fLeft);
            }
            if( s_rctTransistor1.height() < 0 )
            {
                double fTop = s_rctTransistor1.top();
                double fBottom = s_rctTransistor1.bottom();
                s_rctTransistor1.setTop(fBottom);
                s_rctTransistor1.setBottom(fTop);
            }

            strlstExpectedValues.append( strCurrentGraphObj + ": " + polygon2Str(s_rctTransistor1) );

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

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_MovePtBottomRight_Release )

        else if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_Finished )
        {
            pTSGrpTransistorsResize->testEnded();

            m_iTestStepLev3++;  // Next sub test step
            triggerDoTestStep();

        } // if( m_iTestStepLev3 == ETestStepLev3_Transistors_Resize_Transistor1_Finished )

        else // if( m_iTestStepLev3 >= ETestStepLev3_Transistors_Finished )
        {
            pTSGrpTransistors->testEnded();

            m_iTestStepLev2++; // Next test step
            m_iTestStepLev3 = 0;
            triggerDoTestStep();
        }

    } // if( pTSGrpTransistors->isEnabled() )

    delete pMouseEv;
    pMouseEv = nullptr;

} // doTestStepElectricityTransistors

#endif // #if 0
