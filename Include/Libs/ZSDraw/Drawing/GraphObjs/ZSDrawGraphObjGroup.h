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
    void setGridSettings( const CDrawGridSettings& i_gridSettings);
    const CDrawGridSettings& gridSettings() const;
public: // instance methods
    void addToGroup( CGraphObj* i_pGraphObj );
    void removeFromGroup( CGraphObj* i_pGraphObj );
    CGraphObj* findGraphObj( const QString& i_strObjName );
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
    void moveCenter(const QPointF& i_pt);
    void moveCenter(const CPhysValPoint& i_physValPoint);
    CPhysValPoint getCenter() const;
    CPhysValPoint getCenter(const ZS::PhysVal::CUnit& i_unit) const;
    void setTop(double i_fTop);
    void setTop(const ZS::PhysVal::CPhysVal& i_physValTop);
    ZS::PhysVal::CPhysVal getTop() const;
    ZS::PhysVal::CPhysVal getTop(const ZS::PhysVal::CUnit& i_unit) const;
    void setBottom(double i_fBottom);
    void setBottom(const ZS::PhysVal::CPhysVal& i_physValBottom);
    ZS::PhysVal::CPhysVal getBottom() const;
    ZS::PhysVal::CPhysVal getBottom(const ZS::PhysVal::CUnit& i_unit) const;
    void setLeft(double i_fLeft);
    void setLeft(const ZS::PhysVal::CPhysVal& i_physValLeft);
    ZS::PhysVal::CPhysVal getLeft() const;
    ZS::PhysVal::CPhysVal getLeft(const ZS::PhysVal::CUnit& i_unit) const;
    void setRight(double i_fRight);
    void setRight(const ZS::PhysVal::CPhysVal& i_physValRight);
    ZS::PhysVal::CPhysVal getRight() const;
    ZS::PhysVal::CPhysVal getRight(const ZS::PhysVal::CUnit& i_unit) const;
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
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint) const;
    CPhysValPoint convert(const CPhysValPoint& i_physValPoint, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize) const;
    CPhysValSize convert(const CPhysValSize& i_physValSize, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine) const;
    CPhysValLine convert(const CPhysValLine& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
    CPhysValRect convert(const CPhysValRect& i_physValLine) const;
    CPhysValRect convert(const CPhysValRect& i_physValLine, const ZS::PhysVal::CUnit& i_unitDst) const;
public: // must overridables of base class CGraphObj
    virtual CPhysValPoint getPos() const override;
    virtual CPhysValPoint getPos(const ZS::PhysVal::CUnit& i_unit) const override;
public: // must overridables of base class CGraphObj
    virtual QRectF getBoundingRect() const override;
    //virtual QRectF getOriginalBoundingRectInParent() const override;
protected: // must overridables of base class CGraphObj
    virtual void showSelectionPoints( unsigned char i_selPts = ESelectionPointsAll ) override;
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
    virtual void onSelectionPointGeometryChanged(CGraphObj* i_pSelectionPoint);
protected: // auxiliary instance methods (method tracing)
    QRectF setRectOrig(const QRectF& i_rect);
    void applyGeometryChangeToChildrens();
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
         Also used to convert pixel values into metric values and vice versa. */
    ZS::System::GUI::Math::CScaleDivLinesMetrics m_divLinesMetricsY;
    /*!< Settings about the grid lines (visibility, colors, font, etc.). */
    CDrawGridSettings m_gridSettings;
    /*!< The original, untransformed group coordinates with unit.
         TODO: Coordinates are in scene coordinates (relative to top left corner of scene)
               or in parent coordinates (relative to center point of parent group).
               The transformation matrix is used ....
         The coordinates are relative to the top left corner of the parent item's
         bounding rectange (real shape points only). If the item does not have another
         graphical object as a  parent, the coordinates are in scene coordinates.
         If a group is going to be resized at any time the group's width or height may
         become zero. Once the width or height of the group becomes zero resizing the
         children to fit the new groups size would no longer be possible.
         For this the original size of the group must be stored and the current
         scale factor to resize the children is calculated using the current and
         the original size.
         The first "setRect" call whose passed rectangle width and height is not zero
         is taken as the original group size.
         When the group is added to or removed from a another group the current
         coordinates are also taken over as the original coordinates.
         As long as there is no valid original rectangle childrens cannot be resized. */
    QRectF m_rectOrig;
    /*!< The current, untransformed group coordinates with unit.
         The coordinates are relative to the top left corner of the parent item's
         bounding rectange (real shape points only). If the item does not have another
         graphical object as a  parent, the coordinates are in scene coordinates. */
    //CPhysValRect m_physValRectTransformed;

}; // class CGraphObjGroup

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_GraphObjGroup_h
