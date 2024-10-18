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

#include "ZSDraw/Common/ZSDrawPhysValPolyline.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValPolyline
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical polyline on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene),
    m_polygon()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical polyline on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene, i_unit),
    m_polygon()
{
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene, QPolygonF i_polygon) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene),
    m_polygon(std::move(i_polygon))
{
    CPhysValRect::setSize(m_polygon.boundingRect().size());
    CPhysValRect::setCenter(m_polygon.boundingRect().center());
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(
    const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene, i_unit),
    m_polygon(std::move(i_polygon))
{
    CPhysValRect::setSize(m_polygon.boundingRect().size());
    CPhysValRect::setCenter(m_polygon.boundingRect().center());
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CPhysValPolyline& i_physValPolylineOther) :
//------------------------------------------------------------------------------
    CPhysValRect(i_physValPolylineOther),
    m_polygon(i_physValPolylineOther.m_polygon)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline::~CPhysValPolyline()
//------------------------------------------------------------------------------
{
    //m_polygon;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline& CPhysValPolyline::operator = ( const QPolygonF& i_polyline )
//------------------------------------------------------------------------------
{
    m_polygon = i_polyline;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValPolyline::operator = ( const CPhysValShape& i_physValPolylineOther )
//------------------------------------------------------------------------------
{
    dynamic_cast<CPhysValRect&>(*this) = dynamic_cast<const CPhysValRect&>(i_physValPolylineOther);
    const CPhysValPolyline& physValPolylineOther = dynamic_cast<const CPhysValPolyline&>(i_physValPolylineOther);
    m_polygon = physValPolylineOther.m_polygon;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValPolyline::operator == ( const CPhysValShape& i_physValPolylineOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = (dynamic_cast<const CPhysValRect&>(*this) == dynamic_cast<const CPhysValRect&>(i_physValPolylineOther));
    if (bEqual) {
        const CPhysValPolyline& physValPolylineOther = dynamic_cast<const CPhysValPolyline&>(i_physValPolylineOther);
        if (m_polygon.size() != physValPolylineOther.m_polygon.size()) {
            bEqual = false;
        }
        else if (m_unit == physValPolylineOther.m_unit && m_polygon != physValPolylineOther.m_polygon) {
            bEqual = false;
        }
        else if (m_unit != physValPolylineOther.m_unit && m_polygon == physValPolylineOther.m_polygon) {
            bEqual = false;
        }
        else if (m_unit != physValPolylineOther.m_unit && m_polygon != physValPolylineOther.m_polygon) {
            for (int idxPt = 0; idxPt < m_polygon.size(); ++idxPt) {
                if ((*this)[idxPt] != physValPolylineOther[idxPt]) {
                    bEqual = false;
                    break;
                }
            }
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValPolyline::operator != ( const CPhysValShape& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValLineOther);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::operator[](int i_idx) const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_polygon[i_idx]);
}

/*==============================================================================
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the polyline by removing all points.
*/
void CPhysValPolyline::invalidate()
//------------------------------------------------------------------------------
{
    m_polygon = QPolygonF();
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the polyline is valid, otherwise returns false.

    A polyline is always valid, even if it does not contain any points.
*/
bool CPhysValPolyline::isValid() const
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the polyline is null, otherwise returns false.

    A polyline is null if it does not have any points and is empty.
*/
bool CPhysValPolyline::isNull() const
//------------------------------------------------------------------------------
{
    return m_polygon.isEmpty();
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValPolyline::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    i_pPainter->save();
    QPen pen(i_drawSettings.getPenColor());
    pen.setStyle(Qt::SolidLine);
    i_pPainter->setPen(pen);
    QPolygonF polygon_px = m_polygon;
    if (m_unit != Units.Length.px) {
        for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
            polygon_px[idxPt] = m_pDrawingScene->convert((*this)[idxPt], Units.Length.px).toQPointF();
        }
    }
    if (polygon_px.size() > 1) {
        for (int idxPt = 0; idxPt < polygon_px.size()-1; ++idxPt) {
            i_pPainter->drawLine(polygon_px[idxPt], polygon_px[idxPt+1]);
        }
    }
    for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
        i_pPainter->drawText(polygon_px[idxPt], "P" + QString::number(idxPt));
    }
    QPointF ptCenter = m_pDrawingScene->convert(center(), Units.Length.px).toQPointF();
    QPointF ptTL = m_pDrawingScene->convert(topLeft(), Units.Length.px).toQPointF();
    QPointF ptTR = m_pDrawingScene->convert(topRight(), Units.Length.px).toQPointF();
    QPointF ptBR = m_pDrawingScene->convert(bottomRight(), Units.Length.px).toQPointF();
    QPointF ptBL = m_pDrawingScene->convert(bottomLeft(), Units.Length.px).toQPointF();
    QPointF ptTC = m_pDrawingScene->convert(topCenter(), Units.Length.px).toQPointF();
    QPointF ptRC = m_pDrawingScene->convert(rightCenter(), Units.Length.px).toQPointF();
    QPointF ptBC = m_pDrawingScene->convert(bottomCenter(), Units.Length.px).toQPointF();
    QPointF ptLC = m_pDrawingScene->convert(leftCenter(), Units.Length.px).toQPointF();
    pen.setStyle(Qt::DotLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptTL, ptTR);
    i_pPainter->drawLine(ptTR, ptBR);
    i_pPainter->drawLine(ptBR, ptBL);
    i_pPainter->drawLine(ptBL, ptTL);
    pen.setStyle(Qt::DashDotDotLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptTL, ptBR);
    i_pPainter->drawLine(ptTR, ptBL);
    pen.setStyle(Qt::DashLine);
    i_pPainter->setPen(pen);
    i_pPainter->drawLine(ptLC, ptRC);
    i_pPainter->drawLine(ptTC, ptBC);
    if (pen.color() == Qt::red) {
        i_pPainter->drawText(ptTL, "TL");
        i_pPainter->drawText(ptTR, "TR");
        i_pPainter->drawText(ptBR, "BR");
        i_pPainter->drawText(ptBL, "BL");
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
/*! @brief Writes the point to coordinates into a string in the format
           "p1.x, p1.y, p2.x, "p2.y"[ unit.symbol].

    @param [in] i_bAddUnit
        true to append the unit at the end of the coordinates.
    @param [in] i_strSeparator
        String to separate the coordinates
    @param [in] i_iPrecision
        If >= 0 overwrites the precision (number of digits after the decimal point)
        as defined by the internal resolution of the point.
*/
QString CPhysValPolyline::toString(bool i_bAddUnit, const QString& i_strSeparator, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QString str = "[" + QString::number(m_polygon.size()) + "]";
    if (!m_polygon.isEmpty()) {
        str += "(";
        if (i_iPrecision < 0) {
            for (int idxPt = 0; idxPt < m_polygon.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygon[idxPt], i_strSeparator) + "}";
            }
        }
        else {
            for (int idxPt = 0; idxPt < m_polygon.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygon[idxPt], i_strSeparator, 'f', i_iPrecision) + "}";
            }
        }
        str += ")";
    }
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    return str;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief returns the number of polygon points.

    Cannot be named "size" as size is already used by base class CPhysValRect
    returning the size (width and height) of the rectangle.
*/
int CPhysValPolyline::count() const
//------------------------------------------------------------------------------
{
    return m_polygon.size();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The size and rotation angle of the rectangle remain the same.
    The rectangle's edges are implicitly changed.
*/
void CPhysValPolyline::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the rectangle.

    The size and rotation angle of the rectangle remain the same.
    The rectangle's edges are implicitly changed.
*/
void CPhysValPolyline::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setCenter(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the rectangle.

    The size must be passed in the current unit of the rectangle.

    The top left corner and the rotation angle of the rectangle remains the same.
    To keep the top left corner the center point will be moved.

    The rectangle's right and bottom edges are implicitly changed.

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    setSize(CPhysValSize(*m_pDrawingScene, i_size, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the rectangle.

    On changing the size the rotation angle of the rectangle remains the same.

    If the Y axis scale orientation is from top to bottom also the top left
    corner remains unchanged. For scale orientation from bottom to top the
    bottom left corner remains unchanged.

    To keep either the top left or bottom left corner the center point will be moved.

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    CPhysValRect::setSize(i_physValSize);
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The left, top and bottom edges and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edges the center point will be moved.

    The rectangle's right edge is implicitly changed.

    Width must be greater than 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    setWidth(CPhysVal(i_fWidth, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle.

    The left, top and bottom edge and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edge the center point will be moved.

    The rectangle's right edge is implicitly changed.

    Width must be greater or equal than 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    CPhysValRect::setWidth(i_physValWidth);
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the left center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the left center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (right center)
    as well as the top right and bottom right corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setWidthByMovingLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingLeftCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the left center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the left center selection point with mouse events.

    When moving the left center selection point

    - the opposite corner RC must still remain on the same position,
    - the corner points should still be in the order TL, TR, BR, BL if counted clockwise,
    - both the resulting width and height should be greater or equal than 0.

    When moving the left center selection point to a position beyond the right center
    selection point, the rotation angle α must be newly calculated to

    - keep the opposite corner RC' on the same position,
    - to keep the corner points in the order TL, TR, BR, BL if counted clockwise and
    - to keep both the resulting width and height greater or equal than 0.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the selection point to the center point.
*/
void CPhysValPolyline::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setWidthByMovingLeftCenter(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the right center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the right center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (left center)
    as well as the top left and bottom left corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setWidthByMovingRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingRightCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the right center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the right center selection point with mouse events.

    TODO

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setWidthByMovingRightCenter(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.

    The rectangle's bottom edge is implicitly changed.

    Height must be greater than or equal to 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    setHeight(CPhysVal(i_fHeight, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle.

    The left and right edges as well as the rotation angle of the rectangle remains unchanged.

    If the Y axis scale orientation is from top to bottom also the top edge remains unchanged.
    If the Y axis scale orientation is from bottom to top the bottom edge remains unchanged.

    To keep the unchanged edges the center point will be moved.

    Height must be greater than or equal to 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    CPhysValRect::setHeight(i_physValHeight);
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle by moving the top center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the top center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeightByMovingTopCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setHeightByMovingTopCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle by moving the top center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the top center selection point with mouse events.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setHeightByMovingTopCenter(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle by moving the bottom center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the bottom center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (top center)
    as well as the top left and top right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeightByMovingBottomCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setHeightByMovingBottomCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle by moving the bottom center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the bottom center selection point with mouse events.

    The rotation angle, the width and the opposite center point (top center)
    as well as the top left and top right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setHeightByMovingBottomCenter(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the rectangle in degrees.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise with 0° at 3 o'clock (90° at 6 o'clock).

    @param [in] i_physValAngle
        Angle to be set.
*/
void CPhysValPolyline::setAngle( double i_fAngle_degree )
//------------------------------------------------------------------------------
{
    setAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the rectangle in the given unit.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise with 0° at 3 o'clock (90° at 6 o'clock).

    @param [in] i_physValAngle
        Angle to be set. The angle will be normalized to fit into the range 0 .. 360°.
*/
void CPhysValPolyline::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    CPhysValRect::setAngle(i_physValAngle);
}

//------------------------------------------------------------------------------
/*! @brief Sets the top left corner of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle and the opposite corner (bottom right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setTopLeft(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the top left corner of the rectangle.

    The rotation angle and the opposite corner (bottom right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setTopLeft(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the top right corner of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle and the opposite corner (bottom left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setTopRight(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the top right corner of the rectangle.

    The rotation angle and the opposite corner (bottom left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setTopRight(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom right corner of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle and the opposite corner (top left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setBottomRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setBottomRight(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom right corner of the rectangle.

    The rotation angle and the opposite corner (top left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setBottomRight(i_physValPoint);
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom left corner of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle and the opposite corner (top right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setBottomLeft(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom left corner of the rectangle.

    The rotation angle and the opposite corner (top right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolyline::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    CPhysValRect::setBottomLeft(i_physValPoint);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
QPolygonF CPhysValPolyline::toQPolygonF() const
//------------------------------------------------------------------------------
{
    return m_polygon;
}

//------------------------------------------------------------------------------
QPolygonF CPhysValPolyline::toQPolylognF(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPolygonF polyline = m_polygon;
    if (i_unit != m_unit) {
        for (int idxPt = 0; idxPt < polyline.size(); ++idxPt) {
            polyline[idxPt].setX(CPhysVal(polyline[idxPt].x(), m_unit).getVal(i_unit));
            polyline[idxPt].setY(CPhysVal(polyline[idxPt].y(), m_unit).getVal(i_unit));
        }
    }
    return polyline;
}
