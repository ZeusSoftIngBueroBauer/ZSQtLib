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
#include "ZSDraw/Common/ZSDrawAux.h"
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
    CPhysValShape(i_drawingScene),
    m_physValRect(i_drawingScene),
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
    CPhysValShape(i_drawingScene, i_unit),
    m_physValRect(i_drawingScene, i_unit),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(),
    m_polygonModified()
{
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CDrawingScene& i_drawingScene, QPolygonF i_polygon) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_physValRect(i_drawingScene),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonModified()
{
    m_physValRect.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRect.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRect.center().toQPointF();
    m_sizeOrig = m_physValRect.size().toQSizeF();
    m_polygonModified = m_polygonOrig;
}

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(
    const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_physValRect(i_drawingScene, i_unit),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonModified()
{
    m_physValRect.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRect.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRect.center().toQPointF();
    m_sizeOrig = m_physValRect.size().toQSizeF();
    m_polygonModified = m_polygonOrig;
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolyline::CPhysValPolyline(const CPhysValPolyline& i_physValPolylineOther) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValPolylineOther),
    m_physValRect(i_physValPolylineOther.m_physValRect),
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
    m_physValRect.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRect.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRect.center().toQPointF();
    m_sizeOrig = m_physValRect.size().toQSizeF();
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValPolyline::operator = ( const CPhysValShape& i_physValPolylineOther )
//------------------------------------------------------------------------------
{
    const CPhysValPolyline& physValPolylineOther = dynamic_cast<const CPhysValPolyline&>(i_physValPolylineOther);
    m_bYAxisTopDown = physValPolylineOther.m_bYAxisTopDown;
    m_unit = physValPolylineOther.m_unit;
    m_physValRect = physValPolylineOther.m_physValRect;
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
    bool bEqual = true;
    const CPhysValPolyline& physValPolylineOther = dynamic_cast<const CPhysValPolyline&>(i_physValPolylineOther);
    if (!areOfSameUnitGroup(m_unit, physValPolylineOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit != physValPolylineOther.m_unit) {
        bEqual = false;
    }
    else if (m_physValRect != physValPolylineOther.m_physValRect) {
        bEqual = false;
    }
    else if (m_polygonModified.size() != physValPolylineOther.m_polygonModified.size()) {
        bEqual = false;
    }
    else if (m_unit == physValPolylineOther.m_unit && m_polygonModified != physValPolylineOther.m_polygonModified) {
        bEqual = false;
    }
    else if (m_unit != physValPolylineOther.m_unit) {
        for (int idxPt = 0; idxPt < physValPolylineOther.count(); ++idxPt) {
            if (at(idxPt) != physValPolylineOther.at(idxPt)) {
                bEqual = false;
                break;
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
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the polyline by removing all points.
*/
void CPhysValPolyline::invalidate()
//------------------------------------------------------------------------------
{
    m_physValRect.invalidate();
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
            polygon_px[idxPt] = m_pDrawingScene->convert(at(idxPt), Units.Length.px).toQPointF();
        }
    }
    if (polygon_px.size() > 1) {
        for (int idxPt = 0; idxPt < polygon_px.size()-1; ++idxPt) {
            i_pPainter->drawLine(polygon_px[idxPt], polygon_px[idxPt+1]);
        }
    }
    for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
        i_pPainter->drawText(polygon_px[idxPt], "P" + QString::number(idxPt));
        break;
    }
    QPointF ptCenter = m_pDrawingScene->convert(m_physValRect.center(), Units.Length.px).toQPointF();
    QPointF ptTL = m_pDrawingScene->convert(m_physValRect.topLeft(), Units.Length.px).toQPointF();
    QPointF ptTR = m_pDrawingScene->convert(m_physValRect.topRight(), Units.Length.px).toQPointF();
    QPointF ptBR = m_pDrawingScene->convert(m_physValRect.bottomRight(), Units.Length.px).toQPointF();
    QPointF ptBL = m_pDrawingScene->convert(m_physValRect.bottomLeft(), Units.Length.px).toQPointF();
    QPointF ptTC = m_pDrawingScene->convert(m_physValRect.topCenter(), Units.Length.px).toQPointF();
    QPointF ptRC = m_pDrawingScene->convert(m_physValRect.rightCenter(), Units.Length.px).toQPointF();
    QPointF ptBC = m_pDrawingScene->convert(m_physValRect.bottomCenter(), Units.Length.px).toQPointF();
    QPointF ptLC = m_pDrawingScene->convert(m_physValRect.leftCenter(), Units.Length.px).toQPointF();
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
CPhysValPoint CPhysValPolyline::center() const
//------------------------------------------------------------------------------
{
    return m_physValRect.center();
}

//------------------------------------------------------------------------------
CPhysValSize CPhysValPolyline::size() const
//------------------------------------------------------------------------------
{
    return m_physValRect.size();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolyline::width() const
//------------------------------------------------------------------------------
{
    return m_physValRect.width();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolyline::height() const
//------------------------------------------------------------------------------
{
    return m_physValRect.height();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolyline::angle() const
//------------------------------------------------------------------------------
{
    return m_physValRect.angle();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::topLeft() const
//------------------------------------------------------------------------------
{
    return m_physValRect.topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::topRight() const
//------------------------------------------------------------------------------
{
    return m_physValRect.topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::bottomRight() const
//------------------------------------------------------------------------------
{
    return m_physValRect.bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::bottomLeft() const
//------------------------------------------------------------------------------
{
    return m_physValRect.bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::topCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRect.topCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::rightCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRect.rightCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::bottomCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRect.bottomCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolyline::leftCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRect.leftCenter();
}

/*==============================================================================
public: // instance methods
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
    m_physValRect.setCenter(i_physValPoint);
    QPointF ptCenterModified = m_physValRect.center().toQPointF();
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
    m_physValRect.setSize(i_physValSize);
    updateModifiedPolygon();
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
    m_physValRect.setWidth(i_physValWidth);
    updateModifiedPolygon();
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
    m_physValRect.setWidthByMovingLeftCenter(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setWidthByMovingRightCenter(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setHeight(i_physValHeight);
    updateModifiedPolygon();
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
    m_physValRect.setHeightByMovingTopCenter(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setHeightByMovingBottomCenter(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setAngle(i_physValAngle);
    updateModifiedPolygon();
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
    m_physValRect.setTopLeft(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setTopRight(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setBottomRight(i_physValPoint);
    updateModifiedPolygon();
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
    m_physValRect.setBottomLeft(i_physValPoint);
    updateModifiedPolygon();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns true, if the polygon is empty.
*/
bool CPhysValPolyline::isEmpty() const
//------------------------------------------------------------------------------
{
    return m_polygonModified.isEmpty();
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of polygon points.

    Cannot be named "size" as size is already used by base class CPhysValRect
    returning the size (width and height) of the rectangle.
*/
int CPhysValPolyline::count() const
//------------------------------------------------------------------------------
{
    return m_polygonModified.size();
}

//------------------------------------------------------------------------------
/*! @brief Returns the point at index position as a copy.

    Same as method "at(i_idx)".

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.

    @param [in] i_idx
        The index must be a valid index position in the range 0 <= i_idx < count().
*/
CPhysValPoint CPhysValPolyline::at(int i_idx) const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_polygonModified[i_idx]);
}

//------------------------------------------------------------------------------
/*! @brief Replaces the point at the given index.

    When moving a point of the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_idx
        Index of the point to be replaced.
    @param [in] i_physValPoint
        Point to be set.
*/
void CPhysValPolyline::replace(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonModified.replace(i_idx, std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Appends the point at the end of the polygon.

    When adding a point to the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_physValPoint
        Point to be added.
*/
void CPhysValPolyline::append(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonModified.append(std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Inserts the point at the given index to the polygon.

    When adding a point to the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_idx
        Index of the point to be replaced.
    @param [in] i_physValPoint
        Point to be inserted.
*/
void CPhysValPolyline::insert(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonModified.insert(i_idx, std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the given number of points from the polygon starting at the given index.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_idx
        Index where the remove starts.
    @param [in] i_iCount
        Number of points to be removed.
*/
void CPhysValPolyline::remove(int i_idx, int i_iCount)
//------------------------------------------------------------------------------
{
    m_polygonModified.remove(i_idx, i_iCount);
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the point from the polygon at the given index.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_idx
        Index to be removed.
*/
void CPhysValPolyline::removeAt(int i_idx)
//------------------------------------------------------------------------------
{
    m_polygonModified.removeAt(i_idx);
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
void CPhysValPolyline::removeFirst()
//------------------------------------------------------------------------------
{
    m_polygonModified.removeFirst();
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the last point from the polygon.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
void CPhysValPolyline::removeLast()
//------------------------------------------------------------------------------
{
    m_polygonModified.removeLast();
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the point from the polygon at the given index and returns
           the removed point.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.

    @param [in] i_idx
        Index to be removed.
*/
CPhysValPoint CPhysValPolyline::takeAt(int i_idx)
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonModified.takeAt(i_idx);
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon and returns the removed point.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
CPhysValPoint CPhysValPolyline::takeFirst()
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonModified.takeFirst();
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon and returns the removed point.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
CPhysValPoint CPhysValPolyline::takeLast()
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonModified.takeLast();
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polygon as a QPolygonF instance in the current unit.
*/
QPolygonF CPhysValPolyline::toQPolygonF() const
//------------------------------------------------------------------------------
{
    return m_polygonModified;
}

//------------------------------------------------------------------------------
/*! @brief Returns the polygon as a QPolygonF instance in the desired unit.
*/
QPolygonF CPhysValPolyline::toQPolygonF(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPolygonF polygon = m_polygonModified;
    if (i_unit != m_unit) {
        for (int idxPt = 0; idxPt < polygon.size(); ++idxPt) {
            polygon[idxPt].setX(CPhysVal(polygon[idxPt].x(), m_unit).getVal(i_unit));
            polygon[idxPt].setY(CPhysVal(polygon[idxPt].y(), m_unit).getVal(i_unit));
        }
    }
    return polygon;
}

/*==============================================================================
protected: // auxiliary instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal method to update the points in the modified polygon.

    The original points had an original distance vector to the original center point.

    The bounding rectangle of the polygon have have been resized and rotated.
    The resized bounding rectangle has a new, modified center point and a new,
    modified size. The polygon may also be rotated.

    The original distance to the center point will be scaled according to the
    scale factor calculated by the modified and original size. The point will
    then be moved by this distance to the new, modified center point of the
    polygon's bounding rectangle.

    The point will then be rotated by the angle around the new, modified center point.
*/
void CPhysValPolyline::updateModifiedPolygon()
//------------------------------------------------------------------------------
{
    QSizeF sizeModified = m_physValRect.size().toQSizeF();
    QPointF ptCenterModified = m_physValRect.center().toQPointF();
    double fXScaleFactor = sizeModified.width() / m_sizeOrig.width();
    double fYScaleFactor = sizeModified.height() / m_sizeOrig.height();
    for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
        double fdxOrig = m_polygonOrig[idxPt].x() - m_ptCenterOrig.x();
        double fdyOrig = m_polygonOrig[idxPt].y() - m_ptCenterOrig.y();
        double fdxModified = fXScaleFactor * fdxOrig;
        double fdyModified = fYScaleFactor * fdyOrig;
        m_polygonModified[idxPt] = QPointF(ptCenterModified.x() + fdxModified, ptCenterModified.y() + fdyModified);
        if (m_physValRect.angle().getVal() != 0.0) {
            double fAngle_rad = m_physValRect.angle().getVal(Units.Angle.Rad); // clockwise, 0° at 3 o'clock
            fAngle_rad = ZS::System::Math::toCounterClockWiseAngleRad(fAngle_rad);
            if (!m_bYAxisTopDown) {
                // For BottomUp YAxis scale reflection on the x-axis:
                fAngle_rad = Math::c_f360Degrees_rad - fAngle_rad;
            }
            m_polygonModified[idxPt] = ZS::Draw::rotatePoint(ptCenterModified, m_polygonModified[idxPt], fAngle_rad);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal method to update the original polygon coordinates.

    When changing the polygon directly by either modifying points, inserting
    or removing points, the bounding rectangle need to be updated.

    The original coordinates of the bounding rectangle (size, center point)
    will be newly calculated and the modified polygon points will be further
    on used as the original polygon points.

    When changing a point in a rotated polygon, the modified points will first
    be rotated back around the previous center point of the rotated bounding
    rectangle (blue polygon in figure below). Afterwards the bounding rectangle
    of the rotated back polygon is calculated (blue rectangle around blue polygon).

    One of the rotated polygon points is rotated around the center point of the
    blue bounding rectangle. This will result in an offset error between the
    originally modified polygon point (in red color) and the newly calculated,
    modified polygon point (green color).

    The center of the new bounding rectangle will be moved by this offset.
*/
void CPhysValPolyline::updateOriginalPolygon()
//------------------------------------------------------------------------------
{
    // Points may have been added or removed. The modified polygon may have a
    // different size. Just assign the modified to the original coordinates
    // to correct the sizes. For a not rotated polygon thats even the result.
    m_polygonOrig = m_polygonModified;
    double fAngle_rad = 0.0;
    CPhysVal physValAngle = m_physValRect.angle();
    if (physValAngle.getVal() != 0.0) {
        QPointF ptCenterModified = m_physValRect.center().toQPointF();
        fAngle_rad = physValAngle.getVal(Units.Angle.Rad); // clockwise, 0° at 3 o'clock
        fAngle_rad = ZS::System::Math::toCounterClockWiseAngleRad(fAngle_rad);
        if (!m_bYAxisTopDown) {
            // For BottomUp YAxis scale reflection on the x-axis:
            fAngle_rad = Math::c_f360Degrees_rad - fAngle_rad;
        }
        // Rotate back:
        for (int idxPt = 0; idxPt < m_polygonModified.size(); ++idxPt) {
            m_polygonOrig[idxPt] = ZS::Draw::rotatePoint(ptCenterModified, m_polygonModified[idxPt], -fAngle_rad);
        }
    }
    m_physValRect = CPhysValRect(*m_pDrawingScene, m_unit);
    m_physValRect.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRect.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    // Calculate the offset error by rotating one of the original points
    // around the current center of the bounding rectangle.
    if (!m_polygonOrig.isEmpty() && physValAngle.getVal() != 0.0) {
        QPointF ptCenter = m_physValRect.center().toQPointF();
        QPointF ptRotated = ZS::Draw::rotatePoint(ptCenter, m_polygonOrig[0], fAngle_rad);
        QPointF ptOffset = ptRotated - m_polygonModified[0];
        ptCenter -= ptOffset;
        m_physValRect.setCenter(ptCenter);
    }
    m_physValRect.setAngle(physValAngle);
    m_ptCenterOrig = m_physValRect.center().toQPointF();
    m_sizeOrig = m_physValRect.size().toQSizeF();
}
