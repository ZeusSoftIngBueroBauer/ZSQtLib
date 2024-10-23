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

#ifndef ZSDraw_PhysValPolyline_h
#define ZSDraw_PhysValPolyline_h

#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "QtCore/qline.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
/*! @brief Represents a polyline on the drawing scene.

    Corresponds to QPolygonF but providing a physical unit and a resolution.

    As the polygon’s bounding rectangle may be resized by mouse move events,
    width and height of the bounding rectangle may become (at least temporarily) zero.
    To handle several resize events correctly, the original polygon points,
    the original size and the original center point of the bounding rectangle are
    stored to calculate the current scale factor.

    Converting points from pixels to metric units is not supported.
    This conversion would have to be done by the parent group or scene to take
    the different Y-Scale-Axis orientation into account.
    But the polyline does not know about groups.
*/
class ZSDRAWDLL_API CPhysValPolyline : public CPhysValRect
//******************************************************************************
{
public: // class methods
    static QString NameSpace() { return "ZS::Draw"; }
    static QString ClassName() { return "CPhysValPolyline"; }
public: // ctors
    CPhysValPolyline(const CDrawingScene& i_drawingScene);
    CPhysValPolyline(const CDrawingScene& i_drawingScene, const ZS::PhysVal::CUnit& i_unit);
    CPhysValPolyline(const CDrawingScene& i_drawingScene, QPolygonF i_polygon);
    CPhysValPolyline(const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const ZS::PhysVal::CUnit& i_unit);
public: // copy ctor
    CPhysValPolyline(const CPhysValPolyline& i_physValPolylineOther);
public: // dtor
    virtual ~CPhysValPolyline();
public: // operators
    CPhysValPolyline& operator = (const QPolygonF& i_polyline);
public: // must overridable operators of base class CPhysValShape
    CPhysValShape& operator = (const CPhysValShape& i_physValLineOther) override;
public: // must overridables of base class CPhysValShape
    bool operator == (const CPhysValShape& i_physValLineOther) const override;
    bool operator != (const CPhysValShape& i_physValLineOther) const override;
public: // must overridables of base class CPhysValShape
    void invalidate() override;
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ", int i_iPrecision = -1) const override;
public: // overridables of base class CPhysValRect
    void setCenter(const QPointF& i_pt) override;
    void setCenter(const CPhysValPoint& i_physValPoint) override;
    void setSize(const QSizeF& i_size) override;
    void setSize(const CPhysValSize& i_physValSize) override;
    void setWidth(double i_fWidth) override;
    void setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth) override;
    void setWidthByMovingLeftCenter(const QPointF& i_pt) override;
    void setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint) override;
    void setWidthByMovingRightCenter(const QPointF& i_pt) override;
    void setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint) override;
    void setHeight(double i_fHeight) override;
    void setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight) override;
    void setHeightByMovingTopCenter(const QPointF& i_pt) override;
    void setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint) override;
    void setHeightByMovingBottomCenter(const QPointF& i_pt) override;
    void setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint) override;
    void setAngle(double i_fAngle_degree) override;
    void setAngle(const ZS::PhysVal::CPhysVal& i_physValAngle) override;
    void setTopLeft(const QPointF& i_pt) override;
    void setTopLeft(const CPhysValPoint& i_physValPoint) override;
    void setTopRight(const QPointF& i_pt) override;
    void setTopRight(const CPhysValPoint& i_physValPoint) override;
    void setBottomRight(const QPointF& i_pt) override;
    void setBottomRight(const CPhysValPoint& i_physValPoint) override;
    void setBottomLeft(const QPointF& i_pt) override;
    void setBottomLeft(const CPhysValPoint& i_physValPoint) override;
public: // instance methods
    bool isEmpty() const;
    int count() const;
    CPhysValPoint at(int i_idx) const;
    void append(const CPhysValPoint& i_physValPoint);
    void insert(int i_idx, const CPhysValPoint& i_physValPoint);
    void remove(int i_idx, int i_iCount);
    void removeAt(int i_idx);
    void removeFirst();
    void removeLast();
    void replace(int i_idx, const CPhysValPoint& i_physValPoint);
    CPhysValPoint takeAt(int i_idx);
    CPhysValPoint takeFirst();
    CPhysValPoint takeLast();
public: // instance methods (to convert the values into another unit)
    QPolygonF toQPolygonF() const;
    QPolygonF toQPolylognF(const ZS::PhysVal::CUnit& i_unit) const;
protected: // auxiliary instance methods
    void updateModifiedPolygon();
    void updateOriginalPolygon();
protected: // instance members
    /*!< Original center point of the polygons bounding rectangle before resizing events. */
    QPointF m_ptCenterOrig;
    /*!< Original size of the polygons bounding rectangle before resizing events. */
    QSizeF m_sizeOrig;
    /*!< Points of the original polyline stored in "m_unit" before resizing events. */
    QPolygonF m_polygonOrig;
    /*!< Points of the scaled polyline stored in "m_unit" after resizing events. */
    QPolygonF m_polygonModified;

}; // class CPhysValPolyline

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPolyline_h
