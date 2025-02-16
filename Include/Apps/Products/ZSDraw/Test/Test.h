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
#include <QtGui/qpainter.h>
#include <QtGui/qpolygon.h>

#include "ZSTest/ZSTest.h"
#include "ZSDraw/Common/ZSDrawCommon.h"
#include "ZSDraw/Common/ZSDrawUnits.h"

class QGraphicsItem;
class QPainterPath;

#define TEST_ALL 0

#if TEST_ALL == 0

#define TEST_AUX_METHODS 0
#define TEST_DRAWINGSIZE 0
#define TEST_PIXELS_DRAWINGS 1
#define TEST_PIXELS_DRAWINGS_100x100 0
#define TEST_PIXELS_DRAWINGS_101x101 0
#define TEST_PIXELS_DRAWINGS_800x600 1
#define TEST_PIXELS_DRAWINGS_800x600_OJBECT_COORDINATES 0
#define TEST_PIXELS_DRAWINGS_800x600_ADD_OBJECTS 1
#define TEST_PIXELS_DRAWINGS_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_100x100 0
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_OJBECT_COORDINATES 0
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_ADD_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_100x100 0
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_OJBECT_COORDINATES 0
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_ADD_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_DRAW_OBJECTS 1

#define TEST_ADD_OBJECTS_STANDARDSHAPES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_LINES 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_RECTANGLES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_ELLIPSES 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_TEXTS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_POLYGONS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_PLUSSIGNS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_CHECKMARK 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_RECTANGLES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_POLYGONS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_ALL 0

#define TEST_DRAW_OBJECTS_STANDARDSHAPES 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_RECTANGLES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_ELLIPSES 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_TEXTS 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN 0

#else // TEST_ALL

#define TEST_AUX_METHODS 1
#define TEST_DRAWINGSIZE 1
#define TEST_PIXELS_DRAWINGS 1
#define TEST_PIXELS_DRAWINGS_100x100 1
#define TEST_PIXELS_DRAWINGS_101x101 1
#define TEST_PIXELS_DRAWINGS_800x600 1
#define TEST_PIXELS_DRAWINGS_800x600_OJBECT_COORDINATES 1
#define TEST_PIXELS_DRAWINGS_800x600_ADD_OBJECTS 1
#define TEST_PIXELS_DRAWINGS_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_100x100 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_OJBECT_COORDINATES 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_ADD_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_100x100 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_OJBECT_COORDINATES 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_ADD_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_DRAW_OBJECTS 1

#define TEST_ADD_OBJECTS_STANDARDSHAPES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_LINES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_RECTANGLES 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_POLYGONS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_PLUSSIGNS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_LINES_CHECKMARK 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_POLYGONS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_ALL 1

#define TEST_DRAW_OBJECTS_STANDARDSHAPES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_RECTANGLES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_SMALLPLUSSIGN 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_LINES_BIGPLUSSIGN 1

#endif // TEST_ALL

namespace ZS
{
namespace System
{
class CTrcAdminObj;
}
namespace PhysVal
{
class CPhysVal;
}
namespace Draw
{
class CDrawingScene;
class CDrawingView;
class CDrawingSize;
class CDrawGridSettings;
class CGraphObj;
class CPhysValLine;
class CPhysValPoint;
class CPhysValPolygon;
class CPhysValPolyline;
class CPhysValRect;
class CPhysValSize;
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
public: // type definition and constants
    // Lines
    //------
    const QString c_strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    const QString c_strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    const QString c_strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    const QString c_strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
    const QString c_strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    const QString c_strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    const QString c_strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    const QString c_strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    const QString c_strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    const QString c_strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";
    // Rectangles
    //-----------
    const QString c_strGraphObjNameRect = "Rect";
    const QString c_strGraphObjNameBigCrossVerticalBar = "BigCross-VerticalBar";
    const QString c_strGraphObjNameBigCrossHorizontalBar = "BigCross-HorizontalBar";
    const QString c_strGraphObjNameSmallCross1VerticalBar = "SmallCross1-VerticalBar";
    const QString c_strGraphObjNameSmallCross1HorizontalBar = "SmallCross1-HorizontalBar";
    const QString c_strGraphObjNameSmallCross2VerticalBar = "SmallCross2-VerticalBar";
    const QString c_strGraphObjNameSmallCross2HorizontalBar = "SmallCross2-HorizontalBar";
    // Ellipses
    //-----------
    const QString c_strGraphObjNameEllipse = "Ellipse";
    // Texts
    //-----------
    const QString c_strGraphObjNameText = "Text";
    // Polylines
    //----------
    const QString c_strGraphObjNameOpenArrow = "OpenArrow";
    // Polygons
    //---------
    const QString c_strGraphObjNameStar = "Star";
    const QString c_strGraphObjNameTriangle = "Triangle";
    // Groups
    //-------
    const QString c_strGraphObjNameTopGroup = "Top";
    const QString c_strGraphObjNameSmallPlusSign = "SmallPlusSign";
    const QString c_strGraphObjNameBigPlusSign = "BigPlusSign";
    const QString c_strGraphObjNameCheckmark = "Checkmark";
    const QString c_strGraphObjNameSmallRect = "SmallRect";
    const QString c_strGraphObjNameBigCross = "BigCross";
    const QString c_strGraphObjNameSmallCross1 = "SmallCross1";
    const QString c_strGraphObjNameSmallCross2 = "SmallCross2";
    const QString c_strGraphObjNamePolygons = "Polygons";
public: // ctors and dtor
    CTest();
    ~CTest();
public: // instance methods
    void setMainWindow( CMainWindow* i_pMainWindow );
protected: // instance methods
    void createTestGroupPrepareScene(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent,
        const ZS::Draw::CDrawingSize& i_drawingSize,
        const ZS::Draw::CDrawGridSettings& i_gridSettings,
        const QMap<ZS::Draw::EGraphObjType, QPainter::RenderHints> i_mapGraphObjTypeRenderHints = QMap<ZS::Draw::EGraphObjType, QPainter::RenderHints>());
    void createTestGroupAuxMethods(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawingSize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestStepSaveLoadFile(ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int i_iResultValuesPrecision = 3);
    void createTestGroupObjectCoordinatesTransformPhysValShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesTransformPhysValRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesTransformPhysValPolygon(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesAddLines(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesMetricsDrawingConversionFunctions(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesMetricsDrawingConvertToPhysValPoint(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesMetricsDrawingConvertToPhysValSize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesMetricsDrawingConvertToPhysValLine(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupObjectCoordinatesMetricsDrawingConvertToPhysValRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddObjects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesLinesSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesLinesBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesLinesCheckmark(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesLinesSmallRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesRectModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesEllipse(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesEllipseModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesText(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesTextModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonOpenArrow(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonTriangle(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonTriangleModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonStar(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesPlusSigns(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesPlusSignsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesCheckmark(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupLinesCheckmarkModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupRectsCrosses(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupRectsCrossesModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupPolygons(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupPolygonsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesAllGroups(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesAllGroupsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawObjects(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapes(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesLinesSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesLinesBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesLinesCheckmark(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesLinesSmallRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesRect(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesRectModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesEllipse(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesEllipseModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesText(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesTextModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonOpenArrow(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonTriangle(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonTriangleModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonStar(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupLinesSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupLinesSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupLinesBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupLinesBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupLinesBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
protected slots:
    void doTestStepGetLineFromPolar(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepGetPolarCoors(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepGetPerpendicularLine(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawingSize(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepMainWindowSetGeometry(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepClearDrawingScene(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSetDrawingSize(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSetGridSettings(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSetPainterRenderHints(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepTransformPhysValRect(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepTransformPhysValPolygon(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawingSceneConvertToPhysValPoint(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawingSceneConvertToPhysValSize(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawingSceneConvertToPhysValLine(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawingSceneConvertToPhysValRect(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepShowLabels(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepHideLabels(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepShowGeometryLabels(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepHideGeometryLabels(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjLine(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjRect(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjEllipse(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjText(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjGroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjLine(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjRect(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjEllipse(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjText(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjGroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjRectByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjEllipseByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjTextByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjPolylineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSelectAndUngroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSaveLoadFile(ZS::Test::CTestStep* i_pTestStep);
protected: // auxiliary instance methods
    void initInstCounts();
    void initObjectCoors();
    QPoint addMouseMoveEventDataRows(ZS::Test::CTestStep* i_pTestStep, const QPoint& i_ptMousePosStart, const QPoint& i_ptMousePosStop, int i_iMovesCount = 0, Qt::MouseButton mouseBtns = Qt::NoButton, Qt::KeyboardModifiers i_modifiers = Qt::NoModifier);
    QPointF getSelectionPointCoors(const ZS::Draw::CPhysValPolygon& i_physValPolygon, const ZS::Draw::SGraphObjSelectionPoint& i_selPt) const;
    QPointF getSelectionPointCoors(const ZS::Draw::CPhysValRect& i_physValRect, ZS::Draw::ESelectionPoint i_selPt) const;
    //void getSelectionPointCoors(const ZS::Draw::SGraphObjSelectionPoint& i_selPt, const ZS::Draw::CPhysValPolygon& i_physValPolygonCurr, const ZS::Draw::CPhysValPolygon& i_physValPolygonNew, QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew);
    //void getSelectionPointCoors(ZS::Draw::ESelectionPoint i_selPt, const ZS::Draw::CPhysValRect& i_physValRectCurr, const ZS::Draw::CPhysValRect& i_physValRectNew, QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew);
    ZS::System::SErrResultInfo readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const;
    QStringList resultValuesForGraphObj(const ZS::Draw::CGraphObj* i_pGraphObj, bool i_bAddLabelResultValues = false, int i_iPrecision = -1) const;
    QStringList resultValuesForLine(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QLineF& i_lineItemCoors, const ZS::Draw::CPhysValLine& i_physValLine, int i_iPrecision = -1) const;
    QStringList resultValuesForRect(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QRectF& i_rectItemCoors, const ZS::Draw::CPhysValRect& i_physValRect, int i_iPrecision = -1) const;
    QStringList resultValuesForEllipse(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QRectF& i_rectItemCoors, const ZS::Draw::CPhysValRect& i_physValRect, int i_iPrecision = -1) const;
    QStringList resultValuesForText(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QRectF& i_rectItemCoors, const ZS::Draw::CPhysValRect& i_physValRect, int i_iPrecision = -1) const;
    QStringList resultValuesForPolygon(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QPolygonF& i_polygonItemCoors, const ZS::Draw::CPhysValPolygon& i_physValPolygon, int i_iPrecision = -1) const;
    QStringList resultValuesForLabel(
        const QString& strGraphObjName, const QPointF& i_pos, const QString& i_strText, int i_iPrecision = -1) const;
    QStringList resultValuesForGroup(
        const QString& strGraphObjName, const QPointF& i_pos,
        const ZS::Draw::CPhysValRect& i_physValRect, int i_iPrecision = -1) const;
public: // overridables of base class CTest (make protected method public)
    void triggerDoTestStep(int i_iInterval_ms = -1) override;
protected: // class members
    static QHash<QString, int> s_hshGraphObjsInstCounts;
protected: // instance members
    CMainWindow* m_pMainWindow = nullptr;
    ZS::Draw::CDrawingView* m_pDrawingView = nullptr;
    ZS::Draw::CDrawingScene* m_pDrawingScene = nullptr;
    // Map with created objects. Key is name, value is current key in tree.
    // Key in hash is not existing if the object has not been created or has been removed during the test run.
    QMap<QString, QString> m_hshGraphObjNameToKeys;
    // Current mouse position for drawing and modifying objects by mouse events.
    QPoint m_ptMousePos;

    // Lines
    //------
    // SmallPlusSign
    QPointF m_ptPosSmallPlusSignVerticalLine;
    QLineF m_lineSmallPlusSignVerticalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallPlusSignVerticalLine = nullptr;
    QPointF m_ptPosSmallPlusSignHorizontalLine;
    QLineF m_lineSmallPlusSignHorizontalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallPlusSignHorizontalLine = nullptr;
    // BigPlusSign
    QPointF m_ptPosBigPlusSignVerticalLine;
    QLineF m_lineBigPlusSignVerticalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineBigPlusSignVerticalLine = nullptr;
    QPointF m_ptPosBigPlusSignHorizontalLine;
    QLineF m_lineBigPlusSignHorizontalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineBigPlusSignHorizontalLine = nullptr;
    // Checkmark
    QPointF m_ptPosCheckmarkLeftLine;
    QLineF m_lineCheckmarkLeftLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineCheckmarkLeftLine = nullptr;
    QPointF m_ptPosCheckmarkRightLine;
    QLineF m_lineCheckmarkRightLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineCheckmarkRightLine = nullptr;
    // SmallRect
    QPointF m_ptPosSmallRectTopLine;
    QLineF m_lineSmallRectTopLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallRectTopLine = nullptr;
    QPointF m_ptPosSmallRectRightLine;
    QLineF m_lineSmallRectRightLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallRectRightLine = nullptr;
    QPointF m_ptPosSmallRectBottomLine;
    QLineF m_lineSmallRectBottomLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallRectBottomLine = nullptr;
    QPointF m_ptPosSmallRectLeftLine;
    QLineF m_lineSmallRectLeftLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallRectLeftLine = nullptr;

    // Rectangles
    //-----------
    // Rectangle
    QPointF m_ptPosRectangle;
    QRectF m_rectRectangle;
    ZS::Draw::CPhysValRect* m_pPhysValRectRectangle = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleRectangle;
    // Rect-BigCross-VerticalBar
    QPointF m_ptPosRectBigCrossVerticalBar;
    QRectF m_rectRectBigCrossVerticalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigCrossVerticalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigCrossVerticalBar;
    // Rect-BigCross-HorizontalBar
    QPointF m_ptPosRectBigCrossHorizontalBar;
    QRectF m_rectRectBigCrossHorizontalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigCrossHorizontalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigCrossHorizontalBar;
    // Rect-SmallCross1-VerticalBar
    QPointF m_ptPosRectSmallCross1VerticalBar;
    QRectF m_rectRectSmallCross1VerticalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross1VerticalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross1VerticalBar;
    // Rect-SmallCross1-HorizontalBar
    QPointF m_ptPosRectSmallCross1HorizontalBar;
    QRectF m_rectRectSmallCross1HorizontalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross1HorizontalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross1HorizontalBar;
    // Rect-SmallCross2-VerticalBar
    QPointF m_ptPosRectSmallCross2VerticalBar;
    QRectF m_rectRectSmallCross2VerticalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross2VerticalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross2VerticalBar;
    // Rect-SmallCross2-HorizontalBar
    QPointF m_ptPosRectSmallCross2HorizontalBar;
    QRectF m_rectRectSmallCross2HorizontalBar;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross2HorizontalBar = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross2HorizontalBar;

    // Ellipses
    //-----------
    // Ellipse
    QPointF m_ptPosEllipse;
    QRectF m_rectEllipse;
    ZS::Draw::CPhysValRect* m_pPhysValRectEllipse = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleEllipse;

    // Texts
    //-----------
    // Text
    QPointF m_ptPosText;
    QRectF m_rectText;
    ZS::Draw::CPhysValRect* m_pPhysValRectText = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleText;

    // Polygons
    //----------
    // Open Arrow
    QPointF m_ptPosOpenArrow;
    QPolygonF m_polygonOpenArrow;
    ZS::Draw::CPhysValPolygon* m_pPhysValPolygonOpenArrow = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleOpenArrow;
    // Triangle
    QPointF m_ptPosTriangle;
    QPolygonF m_polygonTriangle;
    ZS::Draw::CPhysValPolygon* m_pPhysValPolygonTriangle = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleTriangle;
    // Star
    QPointF m_ptPosStar;
    ZS::Draw::CPhysValPolygon* m_pPhysValPolygonStar = nullptr;
    QPolygonF m_polygonStar;
    ZS::PhysVal::CPhysVal m_physValAngleStar;

    // Groups
    //-------
    // Lines-SmallPlusSign
    QPointF m_ptPosSmallPlusSign;
    QSizeF m_sizeSmallPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallPlusSign;
    // Lines-BigPlusSign
    QPointF m_ptPosBigPlusSign;
    QSizeF m_sizeBigPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigPlusSign;
    // Lines-Checkmark
    QPointF m_ptPosCheckmark;
    QSizeF m_sizeCheckmark;
    ZS::Draw::CPhysValRect* m_pPhysValRectCheckmark = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleCheckmark;
    // Lines-SmallRect
    QPointF m_ptPosSmallRect;
    QSizeF m_sizeSmallRect;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallRect = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallRect;
    // Rects-BigCross
    QPointF m_ptPosBigCross;
    QSizeF m_sizeBigCross;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigCross = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigCross;
    // Rects-SmallCross1
    QPointF m_ptPosSmallCross1;
    QSizeF m_sizeSmallCross1;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross1 = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross1;
    // Rects-SmallCross2
    QPointF m_ptPosSmallCross2;
    QSizeF m_sizeSmallCross2;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallCross2 = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallCross2;
    // Polygons
    QPointF m_ptPosPolygons;
    QSizeF m_sizePolygons;
    ZS::Draw::CPhysValRect* m_pPhysValRectPolygons = nullptr;
    ZS::PhysVal::CPhysVal m_physValAnglePolygons;
    // TopGroup
    QPointF m_ptPosTopGroup;
    QSizeF m_sizeTopGroup;
    ZS::Draw::CPhysValRect* m_pPhysValRectTopGroup = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleTopGroup;

    /*!< Pointer to trace admin object. */
    ZS::System::CTrcAdminObj* m_pTrcAdminObjDrawTestSteps;

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
