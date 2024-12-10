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
#define TEST_PIXELS_DRAWINGS_800x600_ADD_OBJECTS 0
#define TEST_PIXELS_DRAWINGS_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_100x100 0
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_OJBECT_COORDINATES 0
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_ADD_OBJECTS 0
#define TEST_METRICS_DRAWINGS_YSCALE_TOPDOWN_800x600_DRAW_OBJECTS 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_100x100 0
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600 1
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_OJBECT_COORDINATES 0
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_ADD_OBJECTS 0
#define TEST_METRICS_DRAWINGS_YSCALE_BOTTOMUP_800x600_DRAW_OBJECTS 1

#define TEST_ADD_OBJECTS_STANDARDSHAPES 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_LINES 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_POLYGONS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_SMALLPLUSSIGN 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_BIGPLUSSIGN 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_PLUSSIGNS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_CHECKMARK 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_POLYGONS 0
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_ALL 0

#define TEST_DRAW_OBJECTS_STANDARDSHAPES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_SMALLPLUSSIGN 0
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_BIGPLUSSIGN 0

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
#define TEST_ADD_OBJECTS_STANDARDSHAPES_POLYGONS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_SMALLPLUSSIGN 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_BIGPLUSSIGN 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_PLUSSIGNS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_CHECKMARK 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_POLYGONS 1
#define TEST_ADD_OBJECTS_STANDARDSHAPES_GROUPS_ALL 1

#define TEST_DRAW_OBJECTS_STANDARDSHAPES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_LINES 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_POLYGONS 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_SMALLPLUSSIGN 1
#define TEST_DRAW_OBJECTS_STANDARDSHAPES_GROUPS_BIGPLUSSIGN 1

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
    void createTestGroupAddStandardShapesPolygonOpenArrow(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonTriangle(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonTriangleModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesPolygonStar(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupPlusSigns(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupPlusSignsModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupCheckmark(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupAddStandardShapesGroupCheckmarkModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
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
    void createTestGroupDrawStandardShapesPolygonOpenArrow(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonTriangle(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonTriangleModifications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesPolygonStar(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupSmallPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupSmallPlusSignResize(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupBigPlusSign(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupBigPlusSignRotateParent(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
    void createTestGroupDrawStandardShapesGroupBigPlusSignModfications(ZS::Test::CTestStepGroup* i_pTestStepGroupParent);
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
    void doTestStepAddGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepAddGraphObjGroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjLine(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjPolygon(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepDrawGraphObjGroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjLineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjPolylineByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjGroupByDirectMethodCalls(ZS::Test::CTestStep* i_pTestStep);
    // TODO: to be replaced by doTestStepModifyGraphObjByMouseEvents
    void doTestStepModifyGraphObjByMovingSelectionPoints(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepModifyGraphObjByMouseEvents(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSelectAndUngroup(ZS::Test::CTestStep* i_pTestStep);
    void doTestStepSaveLoadFile(ZS::Test::CTestStep* i_pTestStep);
protected: // auxiliary instance methods
    void initInstCounts();
    void initObjectCoors();
    QPoint addMouseMoveEventDataRows(ZS::Test::CTestStep* i_pTestStep, const QPoint& i_ptMousePosStart, const QPoint& i_ptMousePosStop, int i_iMovesCount = 0, Qt::MouseButton mouseBtns = Qt::NoButton, Qt::KeyboardModifiers i_modifiers = Qt::NoModifier);
    void getSelectionPointCoors(const ZS::Draw::SGraphObjSelectionPoint& i_selPt, const ZS::Draw::CPhysValPolygon& i_physValPolygonCurr, const ZS::Draw::CPhysValPolygon& i_physValPolygonNew, QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew);
    void getSelectionPointCoors(ZS::Draw::ESelectionPoint i_selPt, const ZS::Draw::CPhysValRect& i_physValRectCurr, const ZS::Draw::CPhysValRect& i_physValRectNew, QPointF& o_ptSelPtRectCurr, QPointF& o_ptSelPtRectNew);
    ZS::System::SErrResultInfo readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const;
    QStringList resultValuesForGraphObj(const ZS::Draw::CGraphObj* i_pGraphObj, bool i_bAddLabelResultValues = false, int i_iPrecision = -1) const;
    QStringList resultValuesForGroup(
        const QString& strGraphObjName, const QPointF& i_pos,
        const ZS::Draw::CPhysValRect& i_physValRect, int i_iPrecision = -1) const;
    QStringList resultValuesForLine(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QLineF& i_lineItemCoors, const ZS::Draw::CPhysValLine& i_physValLine, int i_iPrecision = -1) const;
    QStringList resultValuesForPolygon(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QPolygonF& i_polygonItemCoors, const ZS::Draw::CPhysValPolygon& i_physValPolygon, int i_iPrecision = -1) const;
    QStringList resultValuesForLabel(
        const QString& strGraphObjName, const QPointF& i_pos, const QString& i_strText, int i_iPrecision = -1) const;
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
    // SmallPlusSign
    QPointF m_ptPosSmallPlusSign;
    QSizeF m_sizeSmallPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallPlusSign;
    // BigPlusSign
    QPointF m_ptPosBigPlusSign;
    QSizeF m_sizeBigPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigPlusSign;
    // Checkmark
    QPointF m_ptPosCheckmark;
    QSizeF m_sizeCheckmark;
    ZS::Draw::CPhysValRect* m_pPhysValRectCheckmark = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleCheckmark;
    // SmallRect
    QPointF m_ptPosSmallRect;
    QSizeF m_sizeSmallRect;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallRect = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallRect;
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

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
