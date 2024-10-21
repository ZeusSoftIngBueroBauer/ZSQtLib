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
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(),
    m_polygonModified()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical polyline on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene, i_unit),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(),
    m_polygonModified()
{
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene, QPolygonF i_polygon) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonModified()
{
    CPhysValRect::setSize(m_polygonOrig.boundingRect().size());
    CPhysValRect::setCenter(m_polygonOrig.boundingRect().center());
    m_ptCenterOrig = m_ptCenter;
    m_sizeOrig = m_size;
    m_polygonModified = m_polygonOrig;
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(
    const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValRect(i_drawingScene, i_unit),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonModified()
{
    CPhysValRect::setSize(m_polygonOrig.boundingRect().size());
    CPhysValRect::setCenter(m_polygonOrig.boundingRect().center());
    m_ptCenterOrig = m_ptCenter;
    m_sizeOrig = m_size;
    m_polygonModified = m_polygonOrig;
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CPhysValPolyline& i_physValPolylineOther) :
//------------------------------------------------------------------------------
    CPhysValRect(i_physValPolylineOther),
    m_ptCenterOrig(i_physValPolylineOther.m_ptCenterOrig),
    m_sizeOrig(i_physValPolylineOther.m_sizeOrig),
    m_polygonOrig(i_physValPolylineOther.m_polygonOrig),
    m_polygonModified(i_physValPolylineOther.m_polygonModified)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline::~CPhysValPolyline()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline& CPhysValPolyline::operator = ( const QPolygonF& i_polyline )
//------------------------------------------------------------------------------
{
    m_polygonOrig = i_polyline;
    m_polygonModified = i_polyline;
    CPhysValRect::setSize(m_polygonOrig.boundingRect().size());
    CPhysValRect::setCenter(m_polygonOrig.boundingRect().center());
    m_ptCenterOrig = m_ptCenter;
    m_sizeOrig = m_size;
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
    m_ptCenterOrig = physValPolylineOther.m_ptCenterOrig;
    m_sizeOrig = physValPolylineOther.m_sizeOrig;
    m_polygonOrig = physValPolylineOther.m_polygonOrig;
    m_polygonModified = physValPolylineOther.m_polygonModified;
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
        if (m_polygonModified.size() != physValPolylineOther.m_polygonModified.size()) {
            bEqual = false;
        }
        else if (m_unit == physValPolylineOther.m_unit && m_polygonModified != physValPolylineOther.m_polygonModified) {
            bEqual = false;
        }
        else if (m_unit != physValPolylineOther.m_unit && m_polygonModified == physValPolylineOther.m_polygonModified) {
            bEqual = false;
        }
        else if (m_unit != physValPolylineOther.m_unit && m_polygonOrig != physValPolylineOther.m_polygonOrig) {
            for (int idxPt = 0; idxPt < physValPolylineOther.count(); ++idxPt) {
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
    return CPhysValPoint(*m_pDrawingScene, m_polygonModified[i_idx]);
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
    CPhysValRect::invalidate();
    m_ptCenterOrig = QPointF();
    m_sizeOrig = QSizeF();
    m_polygonOrig = QPolygonF();
    m_polygonModified = QPolygonF();
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
    return m_polygonModified.isEmpty();
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
    QPolygonF polygon_px = m_polygonModified;
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
/*! @brief Writes the polygon into a human readable string in the format
           "[size] (Idx: {p1.x, p1.y}, ...)[ unit.symbol]".

    @param [in] i_bAddUnit
        true to append the unit at the end of the coordinates.
    @param [in] i_strSeparator
        String to separate the coordinates
    @param [in] i_iPrecision
        If >= 0 overwrites the precision (number of digits after the decimal point)
        as defined by the internal resolution of the drawing scene.
*/
QString CPhysValPolyline::toString(bool i_bAddUnit, const QString& i_strSeparator, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QString str = "[" + QString::number(m_polygonModified.size()) + "]";
    if (!m_polygonModified.isEmpty()) {
        str += "(";
        if (i_iPrecision < 0) {
            for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygonModified[idxPt], i_strSeparator) + "}";
            }
        }
        else {
            for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygonModified[idxPt], i_strSeparator, 'f', i_iPrecision) + "}";
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
    return m_polygonModified.size();
}

/*==============================================================================
public: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the polyline.

    The point must be passed in the current unit of the polyline.

    Invokes setCenter with the current unit.

    @param [in] i_pt
        New center point of the polygon's bounding rectangle.
*/
void CPhysValPolyline::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the polyline.

    To move a polygon the center point of the bounding rectangle is moved.
    The size and rotation angle of the bounding rectangle remain the same.
    The bounding rectangle's edges are implicitly changed.
    Each polygon point will be moved by the same vector as the center point.

    @param [in] i_physValPoint
        New center point of the polygon's bounding rectangle.
*/
void CPhysValPolyline::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValRect::setCenter(i_physValPoint);
    QPointF ptCenterModified = center().toQPointF();
    QPointF ptCenterMove = ptCenterModified - m_ptCenterOrig;
    m_ptCenterOrig = ptCenterModified;
    for (int idxPt = 0; idxPt < m_polygonOrig.size(); ++idxPt) {
        m_polygonOrig[idxPt] += ptCenterMove;
    }
    for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
        m_polygonModified[idxPt] += ptCenterMove;
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the polyline.

    The size must be passed in the current unit of the polyline.

    Invokes setSize with the current unit.

    @param [in] i_size
        New size of the polygon's bounding rectangle.
*/
void CPhysValPolyline::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    setSize(CPhysValSize(*m_pDrawingScene, i_size, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the polyline.

    On changing the size the rotation angle of the polylines bounding rectangle
    remains the same.

    If the Y axis scale orientation is from top to bottom also the top left
    corner remains unchanged. For scale orientation from bottom to top the
    bottom left corner remains unchanged.

    To keep either the top left or bottom left corner the center point will be moved.

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

    As the polyline may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    Each polygon point must be moved by the resize scale factor of the bounding rectangle.

    @param [in] i_physValSize
        New size of the polygon's bounding rectangle.
*/
void CPhysValPolyline::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValSize.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValRect::setSize(i_physValSize);
    QPointF ptCenterModified = center().toQPointF();
    QSizeF sizeModified = size().toQSizeF();
    double fXScaleFactor = sizeModified.width() / m_sizeOrig.width();
    double fYScaleFactor = sizeModified.height() / m_sizeOrig.height();
    for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
        double fdxPrev = m_polygonOrig[idxPt].x() - m_ptCenterOrig.x();
        double fdyPrev = m_polygonOrig[idxPt].y() - m_ptCenterOrig.y();
        double fdxNew = fXScaleFactor * fdxPrev;
        double fdyNew = fYScaleFactor * fdyPrev;
        m_polygonModified[idxPt] = QPointF(ptCenterModified.x() + fdxNew, ptCenterModified.y() + fdyNew);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polyline.

    The value must be passed in the current unit of the rectangle.

*/
void CPhysValPolyline::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    setWidth(CPhysVal(i_fWidth, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polyline.

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
/*! @brief Sets the width of the polyline by moving the left center point.

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
/*! @brief Sets the width of the polyline by moving the left center point.

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
/*! @brief Sets the width of the polyline by moving the right center point.

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
/*! @brief Sets the width of the polyline by moving the right center point.

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
/*! @brief Sets the height of the polyline.

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
/*! @brief Sets the height of the polyline.

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
/*! @brief Sets the height of the polyline by moving the top center point.

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
/*! @brief Sets the height of the polyline by moving the top center point.

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
/*! @brief Sets the height of the polyline by moving the bottom center point.

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
/*! @brief Sets the rotation angle of the polyline in degrees.

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
/*! @brief Sets the rotation angle of the polyline in the given unit.

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
/*! @brief Sets the top left corner of the polyline.

    The point must be passed in the current unit of the polyline.

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
/*! @brief Sets the top left corner of the polyline.

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
/*! @brief Sets the top right corner of the polyline.

    The point must be passed in the current unit of the polyline.

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
/*! @brief Sets the top right corner of the polyline.

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
/*! @brief Sets the bottom right corner of the polyline.

    The point must be passed in the current unit of the polyline.

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
/*! @brief Sets the bottom left corner of the polyline.

    The point must be passed in the current unit of the polyline.

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
/*! @brief Sets the bottom left corner of the polyline.

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
    return m_polygonOrig;
}

//------------------------------------------------------------------------------
QPolygonF CPhysValPolyline::toQPolylognF(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPolygonF polyline = m_polygonOrig;
    if (i_unit != m_unit) {
        for (int idxPt = 0; idxPt < polyline.size(); ++idxPt) {
            polyline[idxPt].setX(CPhysVal(polyline[idxPt].x(), m_unit).getVal(i_unit));
            polyline[idxPt].setY(CPhysVal(polyline[idxPt].y(), m_unit).getVal(i_unit));
        }
    }
    return polyline;
}
