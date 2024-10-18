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
public: // operators
    CPhysValPoint operator[](int i_idx) const;
public: // must overridables of base class CPhysValShape
    void invalidate() override;
    bool isValid() const override;
    bool isNull() const override;
    void draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings) override;
    QString toString(bool i_bAddUnit = false, const QString& i_strSeparator = ", ", int i_iPrecision = -1) const override;
public: // instance methods
    int count() const;
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
public: // instance methods (to convert the values into another unit)
    QPolygonF toQPolygonF() const;
    QPolygonF toQPolylognF(const ZS::PhysVal::CUnit& i_unit) const;
protected: // instance members
    /*!< Points of the polyline stored in "m_unit". */
    QPolygonF m_polygon;

}; // class CPhysValPolyline

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSDraw_PhysValPolyline_h
