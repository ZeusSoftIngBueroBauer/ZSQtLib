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

#ifndef ZSDraw_GraphObjGroup_h
#define ZSDraw_GraphObjGroup_h

#include "ZSDraw/Drawing/GraphObjs/ZSDrawGraphObj.h"
#include "ZSDraw/Common/ZSDrawGridSettings.h"
#include "ZSSysGUI/ZSSysGUIMathScaleDivLinesMetrics.h"

#if QT_VERSION < 0x050000
#include <QtGui/QGraphicsItemGroup>
#else
#include <QtWidgets/QGraphicsItemGroup>
#endif

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Container for graphics items that treats a group of items as a single item.

    A group has a bounding rectangle surrounding all child items.
    The center point of the group is the origin of the group's local coordinate system.

    The origin of the group item's coordinate system is the center of the bounding rectangle.
    The coordinates of the parent's top left corner is not at (0/0) but at negative pixel values.

    +-Drawing-Scene--------------------------------------------------------------------+
    |(0/0)                                                                             |
    |  Scene coordinates     Group's local coordinates   Child's local coordinates     |
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

    If the coordinates of the child have to be entered via gui controls the coordinates
    have to be provided not in the item's local coordinate system but relative to
    the group's top left corner.

    If for example a line belongs to a group, as shown in the picture above, the user enters the
    coordinates (80/30) for P1 and (120/110) for P2 to position the line within the group.
    Those coordinates need to be transformed so that the position of the line becomes (10/10).

    To convert pixel positions into metric values and vice versa the group uses its
    own coordinate system and uses the division line metrics class to do the conversions.
    This also allows drawing own grid division lines within the groups bounding rectangle.
*/
class ZSDRAWDLL_API CGraphObjGroup : public CGraphObj, public QGraphicsItemGroup
//******************************************************************************
{
    Q_OBJECT
public: // class methods
    /*! Returns the namespace the class belongs to. */
    static QString NameSpace() { return "ZS::Draw"; }
    /*! Returns the class name. */
    static QString ClassName() { return "CGraphObjGroup"; }
public: // ctor
    CGraphObjGroup(CDrawingScene* i_pDrawingScene, const QString& i_strObjName = "");
protected: // ctor (used by derived classes)
    CGraphObjGroup(
        CDrawingScene* i_pDrawingScene,
        const QString& i_strFactoryGroupName,
        EGraphObjType i_type,
        const QString& i_strType,
        const QString& i_strObjName);
public: // dtor
    virtual ~CGraphObjGroup();
public: // overridables of base class QGraphicsItem
    virtual int type() const override;
signals:
    /*! Signal emitted if the grid settings has been changed.
        @param i_settings [in] Contains the new grid settings (visibilities of lines, labels, etc.). */
    void gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
public: // must overridables of base class CGraphObj
    virtual CGraphObj* clone() override;
public: // instance methods
    void setGridSettings(const CDrawGridSettings& i_gridSettings);
    const CDrawGridSettings& gridSettings() const;
public: // instance methods
    void addToGroup(CGraphObj* i_pGraphObj);
    void removeFromGroup(CGraphObj* i_pGraphObj);
    void resizeToContent();
    CGraphObj* findGraphObj(const QString& i_strObjName);
    QVector<CGraphObj*> childs() const;
public: // overridables of base class CGraphObj
    virtual QString getScenePolygonShapePointsString() const; // for subsystem test
public: // instance methods
    void setRect(const CPhysValRect& i_physValRect);
    CPhysValRect getRect() const;
    CPhysValRect getRect(const ZS::PhysVal::CUnit& i_unit) const;
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    ZS::PhysVal::CPhysVal getWidth() const;
    ZS::PhysVal::CPhysVal getWidth(const ZS::PhysVal::CUnit& i_unit) const;
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    ZS::PhysVal::CPhysVal getHeight() const;
    ZS::PhysVal::CPhysVal getHeight(const ZS::PhysVal::CUnit& i_unit) const;
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    CPhysValSize getSize() const;
    CPhysValSize getSize(const ZS::PhysVal::CUnit& i_unit) const;
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getCenter() const;
    CPhysValPoint getCenter(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopLeft() const;
    CPhysValPoint getTopLeft(const ZS::PhysVal::CUnit& i_unit) const;
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getTopRight() const;
    CPhysValPoint getTopRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomRight() const;
    CPhysValPoint getBottomRight(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getBottomLeft() const;
    CPhysValPoint getBottomLeft(const ZS::PhysVal::CUnit& i_unit) const;
public: // instance methods
    CPhysValPoint convert(const QPointF& i_pt) const;
    CPhysValPoint convert(const QPointF& i_pt, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const QSizeF& i_size) const;
    CPhysValSize convert(const QSizeF& i_size, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const QLineF& i_line) const;
    CPhysValLine convert(const QLineF& i_line, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect convert(const QRectF& i_rect) const;
    CPhysValRect convert(const QRectF& i_rect, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect convert(const CPhysValRect& i_physValRect) const;
    CPhysValRect convert(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const;
public: // instance methods
    CPhysValPoint mapToScene(const CPhysValPoint& i_physValPoint) const;
    CPhysValPoint mapToScene(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine mapToScene(const CPhysValLine& i_physValLine) const;
    CPhysValLine mapToScene(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect mapToScene(const CPhysValRect& i_physValRect) const;
    CPhysValRect mapToScene(const CPhysValRect& i_physValRect, const ZS::PhysVal::CUnit& i_unitDst) const;
public: // must overridables of base class CGraphObj
    virtual QRectF getBoundingRect() const override;
    virtual QRectF getEffectiveBoundingRectOnScene() const override;
public: // must overridables of base class CGraphObj
    //virtual QRectF getOriginalBoundingRectInParent() const override;
public: // must overridables of base class CGraphObj
    virtual SPolarCoors getPolarCoorsToSelectionPointFromSceneCoors(const QPointF& i_pt, ESelectionPoint i_selPt) const override;
    virtual QLineF getAnchorLineToSelectionPointFromPolarInSceneCoors(const SPolarCoors& i_polarCoors, ESelectionPoint i_selPt) const override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints(TSelectionPointTypes i_selPts = c_uSelectionPointsAll) override;
public: // overridables of base class CGraphObj (text labels)
    virtual QList<SGraphObjSelectionPoint> getPossibleLabelAnchorPoints(const QString& i_strName) const override;
    virtual bool labelHasDefaultValues(const QString& i_strName) const override;
public: // overridables of base class CGraphObj (geometry labels)
    virtual bool geometryLabelHasDefaultValues(const QString& i_strName) const override;
protected: // overridable slots of base class CGraphObj
    virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
public: // must overridables of base class QGraphicsItem
    virtual QRectF boundingRect() const override;
    virtual void paint( QPainter* i_pPainter, const QStyleOptionGraphicsItem* i_pStyleOption, QWidget* i_pWdgt = nullptr ) override;
protected: // overridables of base class QGraphicsItem
    virtual bool sceneEventFilter( QGraphicsItem* i_pGraphicsItemWatched, QEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverMoveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
    virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual void keyPressEvent( QKeyEvent* i_pEv ) override;
    virtual void keyReleaseEvent( QKeyEvent* i_pEv ) override;
protected: // overridables of base class QGraphicsItem
    virtual QVariant itemChange( GraphicsItemChange i_change, const QVariant& i_value ) override;
protected: // overridable slots of base class CGraphObj
    //virtual void onDrawingSizeChanged(const CDrawingSize& i_drawingSize) override;
    virtual void onGraphObjParentGeometryOnSceneChanged(CGraphObj* i_pGraphObjParent);
    virtual void onSelectionPointGeometryOnSceneChanged(CGraphObj* i_pSelectionPoint) override;
protected: // overridables of base class CGraphObj
    virtual void updateOriginalPhysValCoors() override;
protected: // auxiliary instance methods
    void updateDivLinesMetrics(const QSizeF& i_size_px, const QSizeF& i_size_metric);
    QPointF getItemPosAndLocalCoors(const CPhysValRect& i_physValRect, QRectF& o_rect, ZS::PhysVal::CPhysVal& o_physValAngle) const;
    void paintGridLines(QPainter* i_pPainter);
    void paintGridLabelsDivisionLines(QPainter* i_pPainter);
    void paintGridLabels(QPainter* i_pPainter);
protected: // auxiliary instance methods (method tracing)
    CPhysValRect setPhysValRectOrig(const CPhysValRect& i_physValRect);
    QRectF QGraphicsItemGroup_setRect(const QRectF& i_rect);
    void QGraphicsItemGroup_addToGroup(QGraphicsItem* i_pGraphicsItemChild);
    void QGraphicsItemGroup_removeFromGroup(QGraphicsItem* i_pGraphicsItemChild);
    void QGraphicsItem_prepareGeometryChange() override;
protected: // auxiliary instance methods (method tracing)
    void emit_gridSettingsChanged(const ZS::Draw::CDrawGridSettings& i_settings);
public: // overridable auxiliary instance methods of base class CGraphObj (method tracing)
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
    /*!< Mathematic component to calculate the division lines of the X-Scale.
         Also used to convert pixel values into metric values and vice versa. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsX;
    /*!< Mathematic component to calculate the division lines of the Y-Scale.
         Also used to convert pixel values into metric values and vice versa.
         @note The drawing scene's conversion functions cannot be used when converting
               values if the Y-Axis-Scale orientation is set to BottomUp. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsY;
    /*!< Settings about the grid lines (visibility, colors, font, etc.). */
    CDrawGridSettings m_gridSettings;
    /*!< The original, untransformed shape point coordinates in pixels.
         Those are the values in local coordinates relative to the origin of the
         groups bounding rectangle. Other graphics items, like Line, provide methods
         to set and retrieve the local coordinates (e.g. "setLine", "line").
         The group item does not have such methods (e.g. "setRect", "rect").
         To provide the bounding rectangle of the group in local coordinates
         this member is maintained. */
    QRectF m_rectOrig;
    /*!< The original, untransformed shape point coordinates with unit.
         Those are the values in parent coordinates relative to the top left
         or bottom left corner of the parent.
         Untransformed means that the rectangles rotation angle is 0.0
         (separately stored in m_physValRotationAngle of base class CGraphObj)
         and the parent scale factors are not included.
         @see base class CGraphObj "Current and Original Coordinates". */
    CPhysValRect m_physValRectOrig;

}; // class CGraphObjGroup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjGroup_h
