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

#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>

#if QT_VERSION < 0x050000
#include <QtGui/qaction.h>
#else
#include <QtWidgets/qaction.h>
#endif

#include "Test.h"
#include "MainWindow.h"

#include "ZSDraw/ZSDrawAux.h"
#include "ZSDraw/ZSDrawingScene.h"
#include "ZSDraw/ZSDrawingView.h"
#include "ZSDraw/ZSDrawGraphObjPoint.h"
#include "ZSDraw/ZSDrawObjFactory.h"
#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Products::Draw;
using namespace ZS::Trace;


/*******************************************************************************
class CTest : public Test::CTest
*******************************************************************************/

static int s_iPointsCount = 0;

//static QPoint s_ptDrawingSceneMousePos;
//static QPoint s_ptDrawingViewMousePos;

//static const QSize s_arsizePointsCreateFurtherPointsMoveSteps[] = {
//                                /* xOffs, yOffs: xLT, yLT */
//    /*  0 */ QSize(  50,  50 ), /*     0,     0:  50,  50 */ // Coordinates for FirstPoint (m_iMoveStep == 0)
//    /*  1 */ QSize( -15,   0 ), /*   -15,     0:  35,  50 */ // TestStepMin (m_iMoveStep == 1)
//    /*  2 */ QSize(   0,  -5 ), /*   -15,    -5:  35,  45 */
//    /*  3 */ QSize(   0,  -5 ), /*   -15,   -10:  35,  40 */
//    /*  4 */ QSize(   0,  -5 ), /*   -15,   -15:  35,  35 */
//    /*  5 */ QSize(   5,   0 ), /*   -10,   -15:  40,  35 */
//    /*  6 */ QSize(   5,   0 ), /*    -5,   -15:  45,  35 */
//    /*  7 */ QSize(   5,   0 ), /*     0,   -15:  50,  35 */
//    /*  8 */ QSize(   5,   0 ), /*     5,   -15:  55,  35 */
//    /*  9 */ QSize(   5,   0 ), /*    10,   -15:  60,  35 */
//    /* 10 */ QSize(   5,   0 ), /*    15,   -15:  65,  35 */
//    /* 11 */ QSize(   0,   5 ), /*    15,   -10:  65,  40 */
//    /* 12 */ QSize(   0,   5 ), /*    15,    -5:  65,  45 */
//    /* 13 */ QSize(   0,   5 ), /*    15,     0:  65,  50 */
//    /* 14 */ QSize(   0,   5 ), /*    15,     5:  65,  55 */
//    /* 15 */ QSize(   0,   5 ), /*    15,    10:  65,  60 */
//    /* 16 */ QSize(   0,   5 ), /*    15,    15:  65,  65 */
//    /* 17 */ QSize(  -5,   0 ), /*    10,    15:  60,  65 */
//    /* 18 */ QSize(  -5,   0 ), /*     5,    15:  55,  65 */
//    /* 19 */ QSize(  -5,   0 ), /*     0,    15:  50,  65 */
//    /* 20 */ QSize(  -5,   0 ), /*    -5,    15:  45,  65 */
//    /* 21 */ QSize(  -5,   0 ), /*   -10,    15:  40,  65 */
//    /* 22 */ QSize(  -5,   0 ), /*   -15,    15:  35,  65 */
//    /* 23 */ QSize(   0,  -5 ), /*   -15,    10:  35,  60 */
//    /* 24 */ QSize(   0,  -5 )  /*   -15,     5:  35,  55 */ // TestStepMax (m_iMoveStep == 24)
//};

//static const int ETestStepLev3_Points_Create_FurtherPoints_TestStepCount = sizeof(s_arsizePointsCreateFurtherPointsMoveSteps)/sizeof(s_arsizePointsCreateFurtherPointsMoveSteps[0]);

static QList<QPointF> s_arPts;

/*==============================================================================
protected slots: // test step methods
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::createTestGroupMouseEventsStandardShapesPoints( ZS::Test::CTestStepGroup* i_pTestGroupParent, int& io_idxGroup )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    QString strMthOutArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = "IdxGroup:" + QString::number(io_idxGroup);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "createTestGroupMouseEventsStandardShapesPoints",
        /* strAddInfo   */ strMthInArgs );

    ZS::Test::CTestStepGroup* pGrp = new ZS::Test::CTestStepGroup(
        /* pTest           */ this,
        /* strName         */ "Group " + QString::number(++io_idxGroup) + "Point",
        /* pTSGrpParent    */ i_pTestGroupParent );

    int idxStep = 0;

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Clear Drawing Scene",
        /* strOperation    */ "drawing.clear()",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsClearDrawingScene(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Activate Drawing Tool",
        /* strOperation    */ "drawing.setCurrentDrawingTool(Point)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsActivateDrawingTool(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create First Point",
        /* strOperation    */ "view.mousePressEvent(10, 10)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCreateFirstPoint(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create Further Points",
        /* strOperation    */ "view.mouseMoveEvent(10, 10)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCreateFurtherPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Release",
        /* strOperation    */ "view.mouseReleaseEvent()",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsMouseRelease(ZS::Test::CTestStep*)) );

    if( mthTracer.isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthOutArgs = "IdxGroup:" + QString::number(io_idxGroup);
        mthTracer.setMethodOutArgs(strMthOutArgs);
    }

} // createTestGroupMouseEventsStandardShapesPoints

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsClearDrawingScene( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsClearDrawingScene",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues = {"Items: [0]"};

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pDrawingScene->clear();

    QList<QGraphicsItem*> arpItems = m_pDrawingScene->items();

    // Result Values
    //--------------

    QStringList strlstResultValues = {"Items: [" + QString::number(arpItems.size()) + "]"};

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsClearDrawingScene

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsActivateDrawingTool( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsActivateDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
    strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    m_pMainWindow->setCheckedActionDrawStandardShapePoint(true);

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }
    else
    {
        strCurrentDrawingTool = "";
    }

    // Result Values
    //--------------

    QStringList strlstResultValues;

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsActivateDrawingTool

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsCreateFirstPoint( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsCreateFirstPoint",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    /* strOperation might be:

       view.mousePressEvent(10, 10)
    */

    QPoint ptSceneMousePos;

    QString strOperation = i_pTestStep->getOperation();

    if( strOperation.contains("view.mousePressEvent") )
    {
        QString strCoors = strOperation;

        strCoors = strCoors.remove("view.mousePressEvent");
        strCoors = strCoors.remove("(");
        strCoors = strCoors.remove(")");

        QStringList strlstCoors = strCoors.split(",");

        if( strlstCoors.size() == 2 )
        {
            bool bConvertedX = false;
            bool bConvertedY = false;
            int  xCoor = strlstCoors[0].trimmed().toInt(&bConvertedX);
            int  yCoor = strlstCoors[1].trimmed().toInt(&bConvertedY);
            if( bConvertedX && bConvertedY )
            {
                ptSceneMousePos.setX(xCoor);
                ptSceneMousePos.setY(yCoor);
            }
        }
    }

    s_arPts = { ptSceneMousePos };

    s_iPointsCount = 0;

    QPoint ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);

    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QString strCurrentGraphObj = graphObjType2Str(EGraphObjTypePoint) + QString::number(CGraphObjPoint::getCurrentInstCount());

    i_pTestStep->setDescription("scene.create" + graphObjType2Str(EGraphObjTypePoint) + "( ScenePos:" + point2Str(ptSceneMousePos) + " )");

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
    strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));
    strlstExpectedValues.append(strCurrentGraphObj + ": " + point2Str(s_arPts[s_iPointsCount]));

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonPress,
        /* pos       */ ptViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* buttons   */ Qt::NoButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mousePressEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    for( auto& pGraphicsItem : m_pDrawingScene->items() )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

        if( pGraphicsItem != nullptr && pGraphObj != nullptr )
        {
            QPointF ptPos = pGraphicsItem->scenePos();

            if( ptPos == ptSceneMousePos )
            {
                strCurrentGraphObj = pGraphObj->path();
                strlstResultValues.append(strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString());
                break;
            }
        }
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsCreateFirstPoint

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsCreateFurtherPoints( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsCreateFurtherPoints",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    /* strOperation might be:

       view.mouseMoveEvent(10, 10)
    */

    QSize sizeSceneMouseMove;

    QString strOperation = i_pTestStep->getOperation();

    if( strOperation.contains("view.mouseMoveEvent") )
    {
        QString strCoors = strOperation;

        strCoors = strCoors.remove("view.mouseMoveEvent");
        strCoors = strCoors.remove("(");
        strCoors = strCoors.remove(")");

        QStringList strlstCoors = strCoors.split(",");

        if( strlstCoors.size() == 2 )
        {
            bool bConvertedWidth = false;
            bool bConvertedHeight = false;
            int  cxWidth = strlstCoors[0].trimmed().toInt(&bConvertedWidth);
            int  cyHeight = strlstCoors[1].trimmed().toInt(&bConvertedHeight);
            if( bConvertedWidth && bConvertedHeight )
            {
                sizeSceneMouseMove.setWidth(cxWidth);
                sizeSceneMouseMove.setHeight(cyHeight);
            }
        }
    }

    QPoint ptSceneMousePos;

    if( s_arPts.size() > 0 && s_iPointsCount > 0 && s_iPointsCount < s_arPts.size() )
    {
        ptSceneMousePos = m_pDrawingView->mapFromScene(s_arPts[s_iPointsCount]);
    }

    ptSceneMousePos.setX( ptSceneMousePos.x() + sizeSceneMouseMove.width() );
    ptSceneMousePos.setY( ptSceneMousePos.y() + sizeSceneMouseMove.height() );

    s_arPts.append(ptSceneMousePos);

    ++s_iPointsCount;

    QPoint ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);

    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    QString strCurrentGraphObj = graphObjType2Str(EGraphObjTypePoint) + QString::number(CGraphObjPoint::getCurrentInstCount());

    i_pTestStep->setDescription("scene.create" + graphObjType2Str(EGraphObjTypePoint) + "( ScenePos:" + point2Str(ptSceneMousePos) + " )");

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
    strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));
    strlstExpectedValues.append(strCurrentGraphObj + ": " + point2Str(s_arPts[s_iPointsCount]));

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseMove,
        /* pos       */ ptViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::NoButton,
        /* buttons   */ Qt::LeftButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mouseMoveEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    for( auto& pGraphicsItem : m_pDrawingScene->items() )
    {
        CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);

        if( pGraphicsItem != nullptr && pGraphObj != nullptr )
        {
            QPointF ptPos = pGraphicsItem->scenePos();

            if( ptPos == ptSceneMousePos )
            {
                strCurrentGraphObj = pGraphObj->path();
                strlstResultValues.append(strCurrentGraphObj + ": " + pGraphObj->getScenePolygonShapePointsString());
                break;
            }
        }
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsCreateFurtherPoints

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsMouseRelease( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;

    if( m_pTrcAdminObj != nullptr && m_pTrcAdminObj->isActive(ETraceDetailLevelMethodArgs) )
    {
        strMthInArgs = i_pTestStep->path();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ ETraceDetailLevelMethodCalls,
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsMouseRelease",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    QPoint ptSceneMousePos;

    if( s_arPts.size() > 0 && s_iPointsCount > 0 && s_iPointsCount < s_arPts.size() )
    {
        ptSceneMousePos = m_pDrawingView->mapFromScene(s_arPts[s_iPointsCount]);
    }

    QPoint ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);

    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    i_pTestStep->setDescription( "scene.mouseReleaseEvent( ScenePos:" + point2Str(ptSceneMousePos) + " )" );

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
    strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ QEvent::MouseButtonRelease,
        /* pos       */ ptViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ Qt::LeftButton,
        /* buttons   */ Qt::NoButton,
        /* modifiers */ Qt::NoModifier );
    m_pDrawingView->mouseReleaseEvent(pMouseEv);
    delete pMouseEv;
    pMouseEv = nullptr;

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsMouseRelease
