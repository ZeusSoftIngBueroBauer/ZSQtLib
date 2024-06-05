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

#ifndef ZSApps_Draw_Test_h
#define ZSApps_Draw_Test_h

#include <QtCore/qstring.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qsize.h>
#include <QtGui/qpolygon.h>

#include "ZSTest/ZSTest.h"
#include "ZSDraw/Common/ZSDrawCommon.h"

class QGraphicsItem;
class QPainterPath;

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}

namespace Draw
{
class CDrawingScene;
class CDrawingView;
class CDrawingSize;
class CDrawGridSettings;
class CGraphObj;
}

namespace Apps
{
namespace Products
{
namespace Draw
{
class CMainWindow;

//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    static QString NameSpace() { return "ZS::Apps::Products::Draw"; }
public: // ctors and dtor
    CTest();
    ~CTest();
public: // instance methods
    void setMainWindow( CMainWindow* i_pMainWindow );
protected: // instance methods
    ZS::Test::CTestStepGroup* createTestGroupDrawingSize(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupPrepareScene(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize, const ZS::Draw::CDrawGridSettings& i_gridSettings);
    ZS::Test::CTestStepGroup* createTestGroupSaveLoadFile(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinates(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesTransformPhysValShapes(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesTransformPhysValRect(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesPixelsDrawingAddLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingAddLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConversionFunctions(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValPoint(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValSize(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValLine(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupImageSizeAndObjectCoordinatesMetricsDrawingConvertToPhysValRect(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupPixelsDrawing(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupMetricsDrawing(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupMetricsDrawingYScaleTopDown(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupMetricsDrawingYScaleBottomUp(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapes(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapes(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapesLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapesGroups(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupModifyStandardShapesLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupModifyStandardShapesGroups(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapesLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapesGroups(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupDrawMouseEventsCreateObject(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const QString& i_strFactoryGroupName, ZS::Draw::EGraphObjType i_graphObjType, const QString& i_strGraphObjName,
        const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, const QPoint& i_ptMouseAfterRelease);
    ZS::Test::CTestStepGroup* createTestGroupDrawMouseEventsCreateGroup(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const QString& i_strGraphObjName,
        const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, int i_iMovesCount,
        const QPoint& i_ptMouseAfterRelease);
    ZS::Test::CTestStepGroup* createTestGroupShowAndMoveTextLabel(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const QString& i_strGraphObjName, const QString& i_strLabelName,
        const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
    ZS::Test::CTestStepGroup* createTestGroupShowAndMoveGeometryLabel(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const QString& i_strGraphObjName, const QString& i_strLabelName,
        const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
protected: // instance methods
    void addTestStepDrawMouseEventPress(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
        const QPoint& i_ptMousePos);
    void addTestStepDrawMouseEventRelease(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
        const QPoint& i_ptMousePos);
    void addTestStepDrawMouseEventsMove(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
        const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount = 0);
    void addTestStepsDrawMouseEventsMove(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
        const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount = 0);
    void addTestStepsDrawMouseEventsMovePressMoveRelease(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
        const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
protected slots:
    void doTestStepDrawingSize( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMainWindowSetGeometry( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepClearDrawingScene( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetDrawingSize( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetGridSettings( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepTransformPhysValRect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDrawingSceneConvertToPhysValPoint( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDrawingSceneConvertToPhysValSize( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDrawingSceneConvertToPhysValLine( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepDrawingSceneConvertToPhysValRect( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSaveFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepLoadFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetMode( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepUnsetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAddGraphObjLine( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAddGraphObjGroup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyGraphObjLine( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyGraphObjGroup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepShowLabel( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepShowGeometryLabel( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMousePressEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseReleaseEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseMoveEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseMoveEvents( ZS::Test::CTestStep* i_pTestStep );
protected: // auxiliary instance methods
    ZS::System::SErrResultInfo readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const;
protected: // class members
    static QHash<QString, int> s_hshGraphObjsInstCounts;
protected: // instance members
    CMainWindow* m_pMainWindow;
    ZS::Draw::CDrawingView*  m_pDrawingView;
    ZS::Draw::CDrawingScene* m_pDrawingScene;

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
