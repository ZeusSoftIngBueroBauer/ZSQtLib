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

#ifndef ZSDraw_GraphObjLine_h
#define ZSDraw_GraphObjLine_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsLineItem>
#else
#include <QtWidgets/QGraphicsLineItem>
#endif

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Class for line items.

    A line has a start and an end point (P1 and P2).
    The center point of the line is the origin of the line's local coordinate system.

    When positioning a line on the graphics scene or as a child of a parent object
    the setPos method moves the center point of the line.

    If the line has no parent the center point corresponds to the position of
    the line within the graphics scene.

    +-Drawing-Scene--------------------------------------------------------------------+
    |(0/0)                                                                             |
    |  Scene coordinates                                     Line's local coordinates  |
    |                                                                                  |
    |                                                                                  |
    |  (100/50) = scenePos(line.p1)                      + (-20/-40) = line.p1         |
    |                                                     \                            |
    |                                                      \                           |
    |                                                       \                          |
    |  (120/90) = line.pos                                   X (0/0) = line.center()   |
    |                                                         \                        |
    |                                                          \                       |
    |                                                           \                      |
    |  (140/130) = scenePos(line.p2)                             + (20/40) = line.p2   |
    |                                                                                  |
    |                                                                                  |
    +----------------------------------------------------------------------------------+

    If the line has a parent and belongs to a group of items, the origin of the parent
    item's coordinate system is the center of the parent's bounding rectangle. The
    coordinates of the parent's top left corner is not at (0/0) but at negative pixel values.

    +-Drawing-Scene--------------------------------------------------------------------+
    |(0/0)                                                                             |
    |  Scene coordinates     Group's local coordinates   Line's local coordinates      |
    |                                                                                  |
    |                        +---Group---------------x-----------------------+         |
    |                        | (-40/-50) = group.topLeft                     |         |
    |                        |                                               |         |
    |                        |                           + (-20/-40) = line.p1         |
    |                        |                            \                  |         |
    |                        |                       |     \                 |         |
    |  (60/60 = group.pos    x (0/0) = group.center--X--    \                |         |
    |                        | (10/10) = line.pos    |       X (0/0) = line.center     |
    |                        |                                \              |         |
    |                        |                                 \             |         |
    |                        |                                  \            |         |
    |                        | (40/50) = group.bottomRight       + (20/40) = line.p2   |
    |                        +---Group---------------x-----------------------+         |
    |                                                                                  |
    |                                                                                  |
    +----------------------------------------------------------------------------------+

    If the line coordinates have to be entered via gui controls the coordinates
    have to be provided not in the item's local coordinate system but relative to
    the parent's (or drawing scenes) top left corner.

    If the line belongs to a group, as shown in the picture above, the user enters the
    coordinates (80/30) for P1 and (120/110) for P2 to position the line within the group.
    Those coordinates need to be transformed so that the position of the line becomes (10/10).
*/
class ZSDRAWDLL_API CGraphObjLine : public CGraphObj, public QGraphicsLineItem
//******************************************************************************
{
public: // type definitions and constants
    static const QString c_strGeometryLabelNameP1;
    static const QString c_strGeometryLabelNameP2;
    static const QString c_strGeometryLabelNameCenter;
    static const QString c_strGeometryLabelNameDX;
    static const QString c_strGeometryLabelNameDY;
    static const QString c_strGeometryLabelNameLength;
    static const QString c_strGeometryLabelNameAngle;
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjLine"; }
public: // ctors and dtor
    CGraphObjLine(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
    virtual ~CGraphObjLine();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // overridables of base class CGraphObj (for subsystem test)
    virtual QString getScenePolygonShapePointsString() const override;
public: // overridables of base class CGraphObj
    virtual void onCreateAndExecDlgFormatGraphObjs() override;
public: // overridables of base class CGraphObj
    virtual void onDrawSettingsChanged(const CDrawSettings& i_drawSettingsOld) override;
public: // instance methods
    void setLine(const CPhysValLine& i_physValLine);
    void setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const QLineF& i_line, double i_fRes, const ZS::PhysVal::CUnit& i_unit);
    void setLine(const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2);
    CPhysValLine getLine() const;
    CPhysValLine getLine(const ZS::PhysVal::CUnit& i_unit) const;
    void setP1(const QPointF& i_p1);
    void setP1(const CPhysValPoint& i_physValP1);
    CPhysValPoint getP1() const;
    CPhysValPoint getP1(const ZS::PhysVal::CUnit& i_unit) const;
    void setP2(const QPointF& i_p2);
    void setP2(const CPhysValPoint& i_physValP2);
    CPhysValPoint getP2() const;
    CPhysValPoint getP2(const ZS::PhysVal::CUnit& i_unit) const;
    void setCenter(const QPointF& i_pCenter);
    void setCenter(const CPhysValPoint& i_physValCenter);
    CPhysValPoint getCenter() const;
    CPhysValPoint getCenter(const ZS::PhysVal::CUnit& i_unit) const;
    void setLength(double i_fLength);
    void setLength(const ZS::PhysVal::CPhysVal& i_physValLength);
    ZS::PhysVal::CPhysVal getLength() const;
    ZS::PhysVal::CPhysVal getLength(const ZS::PhysVal::CUnit& i_unit) const;
public: // overridables of base class CGraphObj
    void setRotationAngle(const ZS::PhysVal::CPhysVal& i_physValAngle) override;
    ZS::PhysVal::CPhysVal rotationAngle() const override;
    ZS::PhysVal::CPhysVal rotationAngle(const ZS::PhysVal::CUnit& i_unit) const override;
public: // must overridables of base class CGraphObj
    virtual QRectF getBoundingRect() const override;
public: // must overridables of base class CGraphObj
    virtual void updateOriginalPhysValCoors() override;
    //virtual QRectF getOriginalBoundingRectInParent() const override;
    //virtual void setCurrentBoundingRectInParent(const QRectF& i_rectBounding) override;
    //virtual void setGroupScale(double i_fXScale, double i_fYScale) override;
public: // overridables of base class CGraphObj
    virtual QCursor getProposedCursor(const QPointF& i_ptScenePos) const override;
public: // overridables of base class CGraphObj
    virtual CPhysValPoint getPositionOfSelectionPoint(int i_idxPt, const ZS::PhysVal::CUnit& i_unit) const override;
    virtual QPointF getPositionOfSelectionPointInSceneCoors( ESelectionPoint i_selPt ) const override;
    virtual QPointF getPositionOfSelectionPointInSceneCoors( int i_idxPt ) const override;
public: // must overridables of base class CGraphObj
    virtual SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const override;
    virtual SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, int i_idxPt) const override;
    virtual QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, ESelectionPoint i_selPt) const override;
    virtual QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, int i_idxPt) const override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
public: // overridables of base class CGraphObj (text labels)
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
    virtual bool labelHasDefaultValues(const QString& i_strName) const override;
public: // overridables of base class CGraphObj (geometry labels)
    virtual bool geometryLabelHasDefaultValues(const QString& i_strName) const override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter(QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* i_pEv) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* i_pEv) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange(GraphicsItemChange i_change, const QVariant& i_value) override;
protected: // overridable slots of base class CGraphObj
    //virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
    virtual void onSelectionPointGeometryChanged(CGraphObj* i_pSelectionPoint);
protected: // instance methods
    virtual bool lineEndArrowHeadPolygonsNeedUpdate(const CEnumLinePoint& i_linePoint, const CDrawSettings& i_drawSettingsOld) const;
    virtual void updateLineEndArrowHeadPolygons(const CEnumLinePoint& i_linePoint = CEnumLinePoint());
protected: // overridables of base class CGraphObj
    //virtual void updateToolTip() override;
protected: // auxiliary instance methods (method tracing)
    CPhysValLine setLineOrig(const CPhysValLine& i_physValLine);
    QLineF QGraphicsLineItem_setLine(const QLineF& i_line);
    QLineF QGraphicsLineItem_setLine(double i_fX1, double i_fY1, double i_fX2, double i_fY2);
protected: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
    virtual void traceThisPositionInfo(
        ZS::System::CMethodTracer& i_mthTracer,
        ZS::System::EMethodDir i_mthDir = ZS::System::EMethodDir::Undefined,
        ZS::System::ELogDetailLevel i_detailLevel = ZS::System::ELogDetailLevel::Debug) const override;
public: // class members
    /*!< Needed to set an initial unique name when creating a new instance.
         Incremented by the ctor but not decremented by the dtor.
         Used to create a unique name for newly created objects of this type.
         public, so that the test can reset the instance counter to 0. */
    static qint64 s_iInstCount;
protected: // instance members
    /*!< The original, untransformed shape point coordinates with unit.
         @see base class CGraphObj "Current and Original Coordinates". */
    CPhysValLine m_physValLineOrig;
    /*!< The current, untransformed line coordinates with unit.
         The coordinates are relative to the top left corner of the parent item's
         bounding rectange (real shape points only). If the item does not belong as
         a child to a group, the coordinates are in scene coordinates. */
    //CPhysValLine m_physValLineTransformed;
    /*!< Polygon points for arrow head at P1 (line start) */
    QPolygonF m_plgP1ArrowHead;
    /*!< Polygon points for arrow head at P2 (line end) */
    QPolygonF m_plgP2ArrowHead;

}; // class CGraphObjLine

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjLine_h
