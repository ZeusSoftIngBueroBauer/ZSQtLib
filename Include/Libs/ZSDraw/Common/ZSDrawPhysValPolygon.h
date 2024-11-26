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

#ifndef ZSDraw_PhysValPolygon_h
#define ZSDraw_PhysValPolygon_h

#include "ZSDraw/Common/ZSDrawPhysValRect.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a polygon on the drawing scene.

    Corresponds to QPolygonF but providing a physical unit and a resolution.

    When drawing the polygon, depending on the IsClosedPolygon flag, a line will be
    drawn from the last point to the first point.

    As the polygon’s bounding rectangle may be resized by mouse move events,
    width and height of the bounding rectangle may become (at least temporarily) zero.
    To handle several resize events correctly, the original polygon points,
    the original size and the original center point of the bounding rectangle are
    stored to calculate the current scale factor.

    Converting points from pixels to metric units is not supported.
    This conversion would have to be done by the parent group or scene to take
    the different Y-Scale-Axis orientation into account.
    But the polygon shape class does not know about groups.
*/
class ZSDRAWDLL_API CPhysValPolygon : public CPhysValShape
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPolygon"; }
public: // ctors
    CPhysValPolygon(const CDrawingScene& i_drawingScene, bool i_bIsClosedPolygon = true);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit, bool i_bIsClosedPolygon = true);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon, bool i_bIsClosedPolygon = true);
    CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const ZS::PhysVal::CUnit& i_unit, bool i_bIsClosedPolygon = true);
public: // copy ctor
    CPhysValPolygon(const CPhysValPolygon& i_physValPolygonOther);
public: // dtor
    virtual ~CPhysValPolygon();
public: // operators
    CPhysValPolygon& operator = (const QPolygonF& i_polygon);
public: // must overridable operators of base class CPhysValShape
    CPhysValShape& operator = (const CPhysValShape& i_physValPolygonOther) override;
public: // must overridables of base class CPhysValShape
    bool operator == (const CPhysValShape& i_physValPolygonOther) const override;
    bool operator != (const CPhysValShape& i_physValPolygonOther) const override;
public: // must overridables of base class CPhysValShape
    void invalidate() override;
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ", int i_iPrecision = -1) const override;
public: // instance methods
    CPhysValRect physValBoundingRect() const;
    CPhysValPoint center() const;
    CPhysValSize size() const;
    ZS::PhysVal::CPhysVal width() const;
    ZS::PhysVal::CPhysVal height() const;
    ZS::PhysVal::CPhysVal angle() const;
    CPhysValPoint topLeft() const;
    CPhysValPoint topRight() const;
    CPhysValPoint bottomRight() const;
    CPhysValPoint bottomLeft() const;
    CPhysValPoint topCenter() const;
    CPhysValPoint rightCenter() const;
    CPhysValPoint bottomCenter() const;
    CPhysValPoint leftCenter() const;
public: // instance methods
    void setCenter(const QPointF& i_pt);
    void setCenter(const CPhysValPoint& i_physValPoint);
    void setSize(const QSizeF& i_size);
    void setSize(const CPhysValSize& i_physValSize);
    void setWidth(double i_fWidth);
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth);
    void setWidthByMovingLeftCenter(const QPointF& i_pt);
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint);
    void setWidthByMovingRightCenter(const QPointF& i_pt);
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint);
    void setHeight(double i_fHeight);
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight);
    void setHeightByMovingTopCenter(const QPointF& i_pt);
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint);
    void setHeightByMovingBottomCenter(const QPointF& i_pt);
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint);
    void setAngle(double i_fAngle_degree);
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle);
    void setTopLeft(const QPointF& i_pt);
    void setTopLeft(const CPhysValPoint& i_physValPoint);
    void setTopRight(const QPointF& i_pt);
    void setTopRight(const CPhysValPoint& i_physValPoint);
    void setBottomRight(const QPointF& i_pt);
    void setBottomRight(const CPhysValPoint& i_physValPoint);
    void setBottomLeft(const QPointF& i_pt);
    void setBottomLeft(const CPhysValPoint& i_physValPoint);
public: // instance methods
    bool isClosedPolygon() const;
    bool isEmpty() const;
    int count() const;
    CPhysValPoint at(int i_idx) const;
    void replace(int i_idx, const CPhysValPoint& i_physValPoint);
    void append(const CPhysValPoint& i_physValPoint);
    void insert(int i_idx, const CPhysValPoint& i_physValPoint);
    void remove(int i_idx, int i_iCount);
    void removeAt(int i_idx);
    void removeFirst();
    void removeLast();
    CPhysValPoint takeAt(int i_idx);
    CPhysValPoint takeFirst();
    CPhysValPoint takeLast();
public: // instance methods (to convert the values into another unit)
    QPolygonF toQPolygonF() const;
    QPolygonF toQPolygonF(const ZS::PhysVal::CUnit& i_unit) const;
protected: // auxiliary instance methods
    void updateScaledAndRotatedPolygon();
    void updateOriginalPolygon();
protected: // instance members
    /*!< true, if the polygon should be closed by drawing a line from the last point
         to the first point. false otherwise. */
    bool m_bIsClosedPolygon;
    /*!< Original center point of the unscaled and unrotated polygon. */
    QPointF m_ptCenterOrig;
    /*!< Original size of the unscaled and unrotated polygon. */
    QSizeF m_sizeOrig;
    /*!< Original points of the unscaled and unrotated polygon stored in "m_unit". */
    QPolygonF m_polygonOrig;
    /*!< Resized and rotated polygon stored in "m_unit".
         Calculated from the original polygon points applying the scale factor and rotationg angle
         as defined by the resized and rotated rectangle around the polygon. */
    QPolygonF m_polygonScaledAndRotated;
    /*!< Resized and rotated bounding rectangle around the polygon.
         The scale factor of the resized polygon is calculated by thee width and height of the
         scaled and rotated rectangle and the original size of the polygon.
         Please note that if the polygon is rotated "m_polygonScaledAndRotated.boundingRect()"
         returns a rectangle with a different width and hight than "m_physValRectScaledAndRotated". */
    CPhysValRect m_physValRectScaledAndRotated;

}; // class CPhysValPolygon

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPolygon_h
