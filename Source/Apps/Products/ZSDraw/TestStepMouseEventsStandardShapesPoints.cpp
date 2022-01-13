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

#include <QtCore/qcoreapplication.h>
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
#include "ZSSys/ZSSysAux.h"
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

static QList<QPoint> s_arPts;

static int s_iStartTestInstCount = 0;

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
        /* strName         */ "Group " + QString::number(++io_idxGroup) + " Point",
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
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create First Point - MousePress",
        /* strOperation    */ "view.mousePressEvent(30, 30)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create First Point - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(30, 30)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create Further Points - MousePress",
        /* strOperation    */ "view.mousePressEvent(40, 40)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create Further Points - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(50, 50)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Create Further Points - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(50, 50)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Activate Edit Select",
        /* strOperation    */ "drawing.setMode(Select)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsActivateEditSelect(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectByArea - MousePress",
        /* strOperation    */ "view.mousePressEvent(45, 45)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(50, 50)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(55, 55)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectByArea - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(55, 55)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectByArea - Check Selected",
        /* strOperation    */ "Point.isSelected(50, 50): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectOnClick - MousePress",
        /* strOperation    */ "view.mousePressEvent(50, 50)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - SelectOnClick - Check Selected",
        /* strOperation    */ "Point.isSelected(50, 50): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(52, 52)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - Check Selected",
        /* strOperation    */ "Point.isSelected(52, 52): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(55, 55)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - Check Selected",
        /* strOperation    */ "Point.isSelected(55, 55): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(55, 55)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Point - Check Selected",
        /* strOperation    */ "Point.isSelected(55, 55): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MousePress",
        /* strOperation    */ "view.mousePressEvent(20, 20)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - Check Unselected",
        /* strOperation    */ "Point.isSelected(55, 55): false",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(25, 25)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(30, 30)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(40, 40)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(50, 50)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(60, 60)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - SelectByArea - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(60, 60)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - Check Selected",
        /* strOperation    */ "Point.isSelected(30, 30): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - Check Selected",
        /* strOperation    */ "Point.isSelected(40, 40): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Select Points - Check Selected",
        /* strOperation    */ "Point.isSelected(55, 55): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Group Points - Trigger Edit Group",
        /* strOperation    */ "drawing.groupGraphObjsSelected()",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsTriggerEditGroup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Group Points - Check Group Existing and Selected",
        /* strOperation    */ "Group.isSelected(30, 40): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - Select",
        /* strOperation    */ "view.mousePressEvent(43, 43)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(53, 53)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(63, 63)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - MouseMove",
        /* strOperation    */ "view.mouseMoveEvent(73, 73)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - MouseRelease",
        /* strOperation    */ "view.mouseReleaseEvent(73, 73)",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPoints(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Move Group - Check Group Moved",
        /* strOperation    */ "Group.isSelected(73, 73): true",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Ungroup Points - Trigger Edit Ungroup",
        /* strOperation    */ "drawing.ungroupGraphObjsSelected()",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsTriggerEditUngroup(ZS::Test::CTestStep*)) );

    new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Step " + QString::number(++idxStep) + ": Ungroup Group - Check Group Not Existing",
        /* strOperation    */ "?.isSelected(30, 40): ?",
        /* pTSGrpParent    */ pGrp,
        /* szDoTestStepFct */ SLOT(doTestStepMouseEventsStandardShapesPointsCheckResult(ZS::Test::CTestStep*)) );

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

    s_arPts.clear();

    s_iStartTestInstCount = CGraphObjPoint::getCurrentInstCount();

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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesActivateDrawingTool",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
    strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pMainWindow->setCheckedActionDrawStandardShapePoint(true);

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }
    else
    {
        strCurrentDrawingTool = "";
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesActivateDrawingTool

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsActivateEditSelect( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsActivateEditSelect",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::Select).toString());
    strlstExpectedValues.append("DrawingTool: ");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pMainWindow->setCheckedActionEditSelect(true);

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }
    else
    {
        strCurrentDrawingTool = "";
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsActivateEditSelect

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsTriggerEditGroup( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsTriggerEditGroup",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::Select).toString());
    strlstExpectedValues.append("DrawingTool: ");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pMainWindow->triggerActionEditGroup();

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }
    else
    {
        strCurrentDrawingTool = "";
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsTriggerEditGroup

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPointsTriggerEditUngroup( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsTriggerEditUngroup",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::Select).toString());
    strlstExpectedValues.append("DrawingTool: ");

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    m_pMainWindow->triggerActionEditUngroup();

    // Result Values
    //--------------

    QStringList strlstResultValues;

    QString strCurrentDrawingTool;

    if( m_pDrawingScene->getCurrentDrawingTool() != nullptr )
    {
        strCurrentDrawingTool = m_pDrawingScene->getCurrentDrawingTool()->getGraphObjTypeAsString();
    }
    else
    {
        strCurrentDrawingTool = "";
    }

    strlstResultValues.append("EditTool: " + m_pDrawingScene->getEditTool().toString());
    strlstResultValues.append("DrawingTool: " + strCurrentDrawingTool);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsTriggerEditUngroup

//------------------------------------------------------------------------------
/*! Checking the result of selecting graphical objects cannot be done in the same
    test step method in which the according mouse events are processed. The events
    may be queued by Qt's event loop and will not be immediately applied to the objects.
*/
void CTest::doTestStepMouseEventsStandardShapesPointsCheckResult( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPointsCheckResult",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    /* strOperation might be:

       Point.isSelected(45, 45): false
    */

    QPoint ptSceneMousePos;
    QString strIsSelected = "?";
    EGraphObjType graphObjType = EGraphObjTypeUndefined;

    QString strOperation = i_pTestStep->getOperation();

    if( strOperation.contains(".isSelected") )
    {
        QStringList strlstArgs = strOperation.split(":");

        if( strlstArgs.size() == 2 )
        {
            QString strCoors = strlstArgs[0];

            if( strCoors.startsWith("Point") )
            {
                graphObjType = EGraphObjTypePoint;
                strCoors = strCoors.remove("Point.isSelected");
            }
            else if( strCoors.startsWith("Group") )
            {
                graphObjType = EGraphObjTypeGroup;
                strCoors = strCoors.remove("Group.isSelected");
            }
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

            strIsSelected = strlstArgs[1].trimmed();
        }
    } // if( strOperation.contains("point.isSelected") )

    QPoint ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);

    QPoint ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::Select).toString());
    strlstExpectedValues.append("DrawingTool: ");

    QString strCurrentGraphObj = "?.isSelected(): ?";
    if( graphObjType != EGraphObjTypeUndefined )
    {
        strCurrentGraphObj = graphObjType2Str(graphObjType) + ".isSelected(): " + strIsSelected;
    }
    strlstExpectedValues.append(strCurrentGraphObj);

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    CGraphObj*     pGraphObj = nullptr;
    QGraphicsItem* pGraphicsItem = nullptr;

    QList<QGraphicsItem*> arpGraphicsItems = m_pDrawingScene->items(ptSceneMousePos);
    for( auto* pItem : arpGraphicsItems )
    {
        pGraphObj = dynamic_cast<CGraphObj*>(pItem);
        if( pGraphObj != nullptr && pGraphObj->getType() == graphObjType )
        {
            pGraphicsItem = dynamic_cast<QGraphicsItem*>(pGraphObj);
            break;
        }
    }

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

    strCurrentGraphObj = "?.isSelected(): ?";

    if( pGraphObj != nullptr && pGraphicsItem != nullptr )
    {
        strCurrentGraphObj = pGraphObj->getTypeAsString() + ".isSelected(): " + bool2Str(pGraphicsItem->isSelected());
    }
    strlstResultValues.append(strCurrentGraphObj);

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPointsCheckResult

//------------------------------------------------------------------------------
void CTest::doTestStepMouseEventsStandardShapesPoints( ZS::Test::CTestStep* i_pTestStep )
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
        /* strMethod    */ "doTestStepMouseEventsStandardShapesPoints",
        /* strAddInfo   */ strMthInArgs );

    // Expected Values
    //----------------

    QStringList strlstExpectedValues;

    /* strOperation might be:

       view.mouseHoverEvent(50, 50)
       view.mousePressEvent(50, 50)
       view.mouseMoveEvent(50, 50)
       view.mouseReleaseEvent(50, 50)
    */

    QPoint ptSceneMousePos;
    QPoint ptViewMousePos;
    QPoint ptMousePosGlobal;

    QString strTestStep = i_pTestStep->name();
    QString strOperation = i_pTestStep->getOperation();

    QEvent::Type eventType = QEvent::None;
    Qt::MouseButton mouseButton = Qt::NoButton;
    Qt::MouseButtons mouseButtons = Qt::NoButton;

    if( strOperation.contains("view.mouseHoverEvent") )
    {
        QString strCoors = strOperation;

        strCoors = strCoors.remove("view.mouseHoverEvent");
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

        eventType = QEvent::MouseMove;
        mouseButton = Qt::NoButton;
        mouseButtons = Qt::NoButton;

        ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);
        ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

    } // if( strOperation.contains("view.mouseHoverEvent") )

    else if( strOperation.contains("view.mousePressEvent") )
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

        if( strTestStep.contains("Create") )
        {
            s_arPts.append(ptSceneMousePos);
        }

        ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);
        // Strange: but for mouse press and mouse release events the viewport must be used.
        ptMousePosGlobal = m_pDrawingView->viewport()->mapToGlobal(ptViewMousePos);

        eventType = QEvent::MouseButtonPress;
        // I don't know why. But for this test step both mouseButton and mouseButtons must be set to LeftButton.
        mouseButton = Qt::LeftButton;
        mouseButtons = Qt::LeftButton;

    } // if( strOperation.contains("view.mousePressEvent") )

    else if( strOperation.contains("view.mouseMoveEvent") )
    {
        QString strCoors = strOperation;

        strCoors = strCoors.remove("view.mouseMoveEvent");
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

        if( strTestStep.contains("Create") )
        {
            s_arPts.append(ptSceneMousePos);
        }

        ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);
        ptMousePosGlobal = m_pDrawingView->mapToGlobal(ptViewMousePos);

        eventType = QEvent::MouseMove;
        mouseButton = Qt::NoButton;
        mouseButtons = Qt::LeftButton;

    } // if( strOperation.contains("view.mouseMoveEvent") )

    else if( strOperation.contains("view.mouseReleaseEvent") )
    {
        QString strCoors = strOperation;

        strCoors = strCoors.remove("view.mouseReleaseEvent");
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

        ptViewMousePos = m_pDrawingView->mapFromScene(ptSceneMousePos);
        // Strange: but for mouse press and mouse release events the viewport must be used.
        ptMousePosGlobal = m_pDrawingView->viewport()->mapToGlobal(ptViewMousePos);

        eventType = QEvent::MouseButtonRelease;
        mouseButton = Qt::LeftButton;
        mouseButtons = Qt::NoButton;
    }

    ptMousePosGlobal.setX(ptMousePosGlobal.x()+1); // Maybe graphics view or graphics scene bug on calculating the screen position.
    ptMousePosGlobal.setY(ptMousePosGlobal.y()+1); // Without adding 1 pixel the newly created object will not be selected by the scene.

    if( strTestStep.contains("Create") )
    {
        strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::CreateObjects).toString());
        strlstExpectedValues.append("DrawingTool: " + graphObjType2Str(EGraphObjTypePoint));

        int idx = s_iStartTestInstCount;
        for( const auto& pt : s_arPts )
        {
            QString strCurrentGraphObj = graphObjType2Str(EGraphObjTypePoint) + QString::number(idx++);
            strlstExpectedValues.append(strCurrentGraphObj + ": " + point2Str(QPointF(pt)));
        }
    }
    else
    {
        strlstExpectedValues.append("EditTool: " + CEnumEditTool(EEditTool::Select).toString());
        strlstExpectedValues.append("DrawingTool: ");
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);

    // Test Step
    //----------

    QMouseEvent* pMouseEv = new QMouseEvent(
        /* type      */ eventType,
        /* pos       */ ptViewMousePos,
        /* globalPos */ ptMousePosGlobal,
        /* button    */ mouseButton,
        /* buttons   */ mouseButtons,
        /* modifiers */ Qt::NoModifier );
    if( eventType == QEvent::MouseButtonPress )
    {
        m_pDrawingView->mousePressEvent(pMouseEv);
    }
    else if( eventType == QEvent::MouseMove )
    {
        m_pDrawingView->mouseMoveEvent(pMouseEv);
    }
    else if( eventType == QEvent::MouseButtonRelease )
    {
        m_pDrawingView->mouseReleaseEvent(pMouseEv);
    }
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

    if( strTestStep.contains("Create") )
    {
        // Need to sort the list of graphics items return be the scene to follow the order
        // the objects have been created by the test steps.
        QList<QGraphicsItem*> arpGraphicsItems;
        QList<QGraphicsItem*> arpGraphicsItemsTmp = m_pDrawingScene->items();
        int idx = s_iStartTestInstCount;
        for( const auto& pt : s_arPts )
        {
            QString strCurrentGraphObj = graphObjType2Str(EGraphObjTypePoint) + QString::number(idx++);

            for( auto* pGraphicsItem : arpGraphicsItemsTmp )
            {
                CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
                if( pGraphObj->path() == strCurrentGraphObj )
                {
                    arpGraphicsItems.append(pGraphicsItem);
                    arpGraphicsItemsTmp.removeOne(pGraphicsItem);
                    break;
                }
            }
            // Append not expected graphics items to result list.
            while( !arpGraphicsItemsTmp.isEmpty() )
            {
                QGraphicsItem* pGraphicsItem = arpGraphicsItemsTmp.takeLast();
                arpGraphicsItems.append(pGraphicsItem);
                arpGraphicsItemsTmp.removeOne(pGraphicsItem);
            }
        }
        // Graphics items in sorted list are the result values.
        for( auto* pGraphicsItem : arpGraphicsItems )
        {
            CGraphObj* pGraphObj = dynamic_cast<CGraphObj*>(pGraphicsItem);
            if( pGraphicsItem != nullptr && pGraphObj != nullptr )
            {
                strlstResultValues.append(pGraphObj->path() + ": " + pGraphObj->getScenePolygonShapePointsString());
            }
        }
    }

    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMouseEventsStandardShapesPoints
