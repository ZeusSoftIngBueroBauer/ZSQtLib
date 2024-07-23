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
#include "ZSDraw/Common/ZSDrawUnits.h"

class QGraphicsItem;
class QPainterPath;

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
    const QString c_strGraphObjNameTopGroup = "Top";
    const QString c_strGraphObjNamePlusSigns = "PlusSigns";
    const QString c_strGraphObjNameSmallPlusSign = "SmallPlusSign";
    const QString c_strGraphObjNameSmallPlusSignVerticalLine = "SmallPlusSign-VerticalLine";
    const QString c_strGraphObjNameSmallPlusSignHorizontalLine = "SmallPlusSign-HorizontalLine";
    const QString c_strGraphObjNameBigPlusSign = "BigPlusSign";
    const QString c_strGraphObjNameBigPlusSignVerticalLine = "BigPlusSign-VerticalLine";
    const QString c_strGraphObjNameBigPlusSignHorizontalLine = "BigPlusSign-HorizontalLine";
    const QString c_strGraphObjNameCheckmark = "Checkmark";
    const QString c_strGraphObjNameCheckmarkLeftLine = "Checkmark-LeftLine";
    const QString c_strGraphObjNameCheckmarkRightLine = "Checkmark-RightLine";
    const QString c_strGraphObjNameSmallRect = "SmallRect";
    const QString c_strGraphObjNameSmallRectTopLine = "SmallRect-TopLine";
    const QString c_strGraphObjNameSmallRectRightLine = "SmallRect-RightLine";
    const QString c_strGraphObjNameSmallRectBottomLine = "SmallRect-BottomLine";
    const QString c_strGraphObjNameSmallRectLeftLine = "SmallRect-LeftLine";
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
    ZS::Test::CTestStepGroup* createTestGroupAddObjects(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const QStringList& i_strlstGraphObjGroupNames);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapes(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize, const QStringList& i_strlstGraphObjGroupNames);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapesLines(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize, const QStringList& i_strlstGraphObjGroupNames);
    ZS::Test::CTestStepGroup* createTestGroupAddStandardShapesGroups(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize, const QStringList& i_strlstGraphObjGroupNames);
    ZS::Test::CTestStepGroup* createTestGroupModifyStandardShapesBigPlusSign(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize);
    ZS::Test::CTestStepGroup* createTestGroupModifyStandardShapesAll(
        ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
        const ZS::Draw::CDrawingSize& i_drawingSize, const QStringList& i_strlstGraphObjGroupNames);

    //ZS::Test::CTestStepGroup* createTestGroupDrawObjects(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    //ZS::Test::CTestStepGroup* createTestGroupPixelsDrawing(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    //ZS::Test::CTestStepGroup* createTestGroupMetricsDrawing(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    //ZS::Test::CTestStepGroup* createTestGroupMetricsDrawingYScaleTopDown(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    //ZS::Test::CTestStepGroup* createTestGroupMetricsDrawingYScaleBottomUp(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup);
    //ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapes(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const ZS::Draw::CDrawingSize& i_drawingSize);
    //ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapesLines(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const ZS::Draw::CDrawingSize& i_drawingSize);
    //ZS::Test::CTestStepGroup* createTestGroupDrawStandardShapesGroups(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const ZS::Draw::CDrawingSize& i_drawingSize);
    //ZS::Test::CTestStepGroup* createTestGroupDrawMouseEventsCreateObject(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const QString& i_strFactoryGroupName, ZS::Draw::EGraphObjType i_graphObjType, const QString& i_strGraphObjName,
    //    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, const QPoint& i_ptMouseAfterRelease);
    //ZS::Test::CTestStepGroup* createTestGroupDrawMouseEventsCreateGroup(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const QString& i_strGraphObjName,
    //    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease, int i_iMovesCount,
    //    const QPoint& i_ptMouseAfterRelease);
    //ZS::Test::CTestStepGroup* createTestGroupShowAndMoveTextLabel(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const QString& i_strGraphObjName, const QString& i_strLabelName,
    //    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
    //ZS::Test::CTestStepGroup* createTestGroupShowAndMoveGeometryLabel(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxGroup,
    //    const QString& i_strGraphObjName, const QString& i_strLabelName,
    //    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
protected: // instance methods
    //void addTestStepDrawMouseEventPress(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    //    const QPoint& i_ptMousePos);
    //void addTestStepDrawMouseEventRelease(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    //    const QPoint& i_ptMousePos);
    //void addTestStepDrawMouseEventsMove(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    //    const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount = 0);
    //void addTestStepsDrawMouseEventsMove(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    //    const QPoint& i_ptMouseStart, const QPoint& i_ptMouseStop, int i_iMovesCount = 0);
    //void addTestStepsDrawMouseEventsMovePressMoveRelease(
    //    ZS::Test::CTestStepGroup* i_pTestStepGroupParent, int& io_idxStep,
    //    const QPoint& i_ptMousePress, const QPoint& i_ptMouseRelease);
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
    void doTestStepSaveLoadFile( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetMode( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepSetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepUnsetCurrentDrawingTool( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAddGraphObjLine( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepAddGraphObjGroup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyGraphObjLine( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepModifyGraphObjGroup( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepShowLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepHideLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepShowGeometryLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepHideGeometryLabels( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMousePressEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseReleaseEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseMoveEvent( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepMouseMoveEvents( ZS::Test::CTestStep* i_pTestStep );
protected: // auxiliary instance methods
    ZS::System::SErrResultInfo readFile(const QString& i_strAbsFilePath, QStringList& o_strlstLines) const;
    QStringList resultValuesForGraphObj(const ZS::Draw::CGraphObj* i_pGraphObj, bool i_bAddLabelResultValues = false) const;
    QStringList resultValuesForGroup(
        const QString& strGraphObjName, const QPointF& i_pos,
        const ZS::Draw::CPhysValRect& i_physValRect) const;
    QStringList resultValuesForLine(
        const QString& strGraphObjName, const QPointF& i_pos,
        const QLineF& i_line, const ZS::Draw::CPhysValLine& i_physValLine) const;
    QStringList resultValuesForLabel(
        const QString& strGraphObjName, const QPointF& i_pos, const QString& i_strText) const;
protected: // class members
    static QHash<QString, int> s_hshGraphObjsInstCounts;
protected: // instance members
    CMainWindow* m_pMainWindow = nullptr;
    ZS::Draw::CDrawingView* m_pDrawingView = nullptr;
    ZS::Draw::CDrawingScene* m_pDrawingScene = nullptr;
    // SmallPlusSign
    QPointF m_ptPosSmallPlusSign;
    QSizeF m_sizeSmallPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallPlusSign;
    QPointF m_ptPosSmallPlusSignVerticalLine;
    QLineF m_lineSmallPlusSignVerticalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallPlusSignVerticalLine = nullptr;
    QPointF m_ptPosSmallPlusSignHorizontalLine;
    QLineF m_lineSmallPlusSignHorizontalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineSmallPlusSignHorizontalLine = nullptr;
    // BigPlusSign
    QPointF m_ptPosBigPlusSign;
    QSizeF m_sizeBigPlusSign;
    ZS::Draw::CPhysValRect* m_pPhysValRectBigPlusSign = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleBigPlusSign;
    QPointF m_ptPosBigPlusSignVerticalLine;
    QLineF m_lineBigPlusSignVerticalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineBigPlusSignVerticalLine = nullptr;
    QPointF m_ptPosBigPlusSignHorizontalLine;
    QLineF m_lineBigPlusSignHorizontalLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineBigPlusSignHorizontalLine = nullptr;
    // PlusSigns
    QPointF m_ptPosPlusSigns;
    QSizeF m_sizePlusSigns;
    ZS::Draw::CPhysValRect* m_pPhysValRectPlusSigns = nullptr;
    ZS::PhysVal::CPhysVal m_physValAnglePlusSigns;
    // Checkmark
    QPointF m_ptPosCheckmark;
    QSizeF m_sizeCheckmark;
    ZS::Draw::CPhysValRect* m_pPhysValRectCheckmark = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleCheckmark;
    QPointF m_ptPosCheckmarkLeftLine;
    QLineF m_lineCheckmarkLeftLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineCheckmarkLeftLine = nullptr;
    QPointF m_ptPosCheckmarkRightLine;
    QLineF m_lineCheckmarkRightLine;
    ZS::Draw::CPhysValLine* m_pPhysValLineCheckmarkRightLine = nullptr;
    // SmallRect
    QPointF m_ptPosSmallRect;
    QSizeF m_sizeSmallRect;
    ZS::Draw::CPhysValRect* m_pPhysValRectSmallRect = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleSmallRect;
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
    // TopGroup
    QPointF m_ptPosTopGroup;
    QSizeF m_sizeTopGroup;
    ZS::Draw::CPhysValRect* m_pPhysValRectTopGroup = nullptr;
    ZS::PhysVal::CPhysVal m_physValAngleTopGroup;
    // Map with created objects. Key is name, value is current key in tree.
    // Key in hash is not existing if the object has not been created or has been removed during the test run.
    QMap<QString, QString> m_hshGraphObjNameToKeys;

}; // class CTest

} // namespace Draw

} // namespace Products

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_Draw_Test_h
