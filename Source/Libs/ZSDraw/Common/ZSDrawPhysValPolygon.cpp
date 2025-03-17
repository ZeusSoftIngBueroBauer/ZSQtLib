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

#include "ZSDraw/Common/ZSDrawPhysValPolygon.h"
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
class CPhysValPolygon
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical polygon on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene, bool i_bIsClosedPolygon) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_bIsClosedPolygon(i_bIsClosedPolygon),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(),
    m_polygonScaledAndRotated(),
    m_physValRectScaledAndRotated(i_drawingScene)
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical polygon on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene, const CUnit& i_unit, bool i_bIsClosedPolygon) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_bIsClosedPolygon(i_bIsClosedPolygon),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(),
    m_polygonScaledAndRotated(),
    m_physValRectScaledAndRotated(i_drawingScene, i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(const CDrawingScene& i_drawingScene, QPolygonF i_polygon, bool i_bIsClosedPolygon) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_bIsClosedPolygon(i_bIsClosedPolygon),
    m_ptCenterOrig(),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonScaledAndRotated(),
    m_physValRectScaledAndRotated(i_drawingScene)
{
    m_physValRectScaledAndRotated.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRectScaledAndRotated.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRectScaledAndRotated.center().toQPointF();
    m_sizeOrig = m_physValRectScaledAndRotated.size().toQSizeF();
    m_polygonScaledAndRotated = m_polygonOrig;
}

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(
    const CDrawingScene& i_drawingScene, QPolygonF i_polygon, const CUnit& i_unit, bool i_bIsClosedPolygon) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_bIsClosedPolygon(i_bIsClosedPolygon),
    m_sizeOrig(),
    m_polygonOrig(std::move(i_polygon)),
    m_polygonScaledAndRotated(),
    m_physValRectScaledAndRotated(i_drawingScene, i_unit)
{
    m_physValRectScaledAndRotated.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRectScaledAndRotated.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRectScaledAndRotated.center().toQPointF();
    m_sizeOrig = m_physValRectScaledAndRotated.size().toQSizeF();
    m_polygonScaledAndRotated = m_polygonOrig;
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon::CPhysValPolygon(const CPhysValPolygon& i_physValPolygonOther) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValPolygonOther),
    m_bIsClosedPolygon(i_physValPolygonOther.m_bIsClosedPolygon),
    m_ptCenterOrig(i_physValPolygonOther.m_ptCenterOrig),
    m_sizeOrig(i_physValPolygonOther.m_sizeOrig),
    m_polygonOrig(i_physValPolygonOther.m_polygonOrig),
    m_polygonScaledAndRotated(i_physValPolygonOther.m_polygonScaledAndRotated),
    m_physValRectScaledAndRotated(i_physValPolygonOther.m_physValRectScaledAndRotated)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon::~CPhysValPolygon()
//------------------------------------------------------------------------------
{
    m_bIsClosedPolygon = false;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPolygon& CPhysValPolygon::operator = ( const QPolygonF& i_polygon )
//------------------------------------------------------------------------------
{
    m_polygonOrig = i_polygon;
    m_polygonScaledAndRotated = i_polygon;
    m_physValRectScaledAndRotated.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRectScaledAndRotated.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    m_ptCenterOrig = m_physValRectScaledAndRotated.center().toQPointF();
    m_sizeOrig = m_physValRectScaledAndRotated.size().toQSizeF();
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValPolygon::operator = ( const CPhysValShape& i_physValPolygonOther )
//------------------------------------------------------------------------------
{
    const CPhysValPolygon& physValPolygonOther = dynamic_cast<const CPhysValPolygon&>(i_physValPolygonOther);
    m_bYAxisTopDown = physValPolygonOther.m_bYAxisTopDown;
    m_unit = physValPolygonOther.m_unit;
    m_bIsClosedPolygon = physValPolygonOther.m_bIsClosedPolygon;
    m_physValRectScaledAndRotated = physValPolygonOther.m_physValRectScaledAndRotated;
    m_ptCenterOrig = physValPolygonOther.m_ptCenterOrig;
    m_sizeOrig = physValPolygonOther.m_sizeOrig;
    m_polygonOrig = physValPolygonOther.m_polygonOrig;
    m_polygonScaledAndRotated = physValPolygonOther.m_polygonScaledAndRotated;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValPolygon::operator == ( const CPhysValShape& i_physValPolygonOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    const CPhysValPolygon& physValPolygonOther = dynamic_cast<const CPhysValPolygon&>(i_physValPolygonOther);
    if (!areOfSameUnitGroup(m_unit, physValPolygonOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit != physValPolygonOther.m_unit) {
        bEqual = false;
    }
    else if (m_bIsClosedPolygon != physValPolygonOther.m_bIsClosedPolygon) {
        bEqual = false;
    }
    else if (m_physValRectScaledAndRotated != physValPolygonOther.m_physValRectScaledAndRotated) {
        bEqual = false;
    }
    else if (m_polygonScaledAndRotated.size() != physValPolygonOther.m_polygonScaledAndRotated.size()) {
        bEqual = false;
    }
    else if (m_unit == physValPolygonOther.m_unit && m_polygonScaledAndRotated != physValPolygonOther.m_polygonScaledAndRotated) {
        bEqual = false;
    }
    else if (m_unit != physValPolygonOther.m_unit) {
        for (int idxPt = 0; idxPt < physValPolygonOther.count(); ++idxPt) {
            if (at(idxPt) != physValPolygonOther.at(idxPt)) {
                bEqual = false;
                break;
            }
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValPolygon::operator != ( const CPhysValShape& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValLineOther);
}

/*==============================================================================
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the polygon by removing all points.
*/
void CPhysValPolygon::invalidate()
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.invalidate();
    m_ptCenterOrig = QPointF();
    m_sizeOrig = QSizeF();
    m_polygonOrig = QPolygonF();
    m_polygonScaledAndRotated = QPolygonF();
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the polygon is valid, otherwise returns false.

    A polygon is always valid, even if it does not contain any points.
*/
bool CPhysValPolygon::isValid() const
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the polygon is null, otherwise returns false.

    A polygon is null if it does not have any points and is empty.
*/
bool CPhysValPolygon::isNull() const
//------------------------------------------------------------------------------
{
    return m_polygonScaledAndRotated.isEmpty();
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValPolygon::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    i_pPainter->save();
    QPen pen(i_drawSettings.penColor());
    pen.setStyle(Qt::SolidLine);
    i_pPainter->setPen(pen);
    QPolygonF polygon_px = m_polygonScaledAndRotated;
    if (m_unit != Units.Length.px) {
        for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
            polygon_px[idxPt] = m_pDrawingScene->convert(at(idxPt), Units.Length.px).toQPointF();
        }
    }
    if (polygon_px.size() > 1) {
        if (m_bIsClosedPolygon) {
            i_pPainter->drawPolygon(polygon_px);
        }
        else {
            i_pPainter->drawPolyline(polygon_px);
        }
    }
    for (int idxPt = 0; idxPt < polygon_px.size(); ++idxPt) {
        i_pPainter->drawText(polygon_px[idxPt], "P" + QString::number(idxPt));
        break;
    }
    QPointF ptCenter = m_pDrawingScene->convert(m_physValRectScaledAndRotated.center(), Units.Length.px).toQPointF();
    QPointF ptTL = m_pDrawingScene->convert(m_physValRectScaledAndRotated.topLeft(), Units.Length.px).toQPointF();
    QPointF ptTR = m_pDrawingScene->convert(m_physValRectScaledAndRotated.topRight(), Units.Length.px).toQPointF();
    QPointF ptBR = m_pDrawingScene->convert(m_physValRectScaledAndRotated.bottomRight(), Units.Length.px).toQPointF();
    QPointF ptBL = m_pDrawingScene->convert(m_physValRectScaledAndRotated.bottomLeft(), Units.Length.px).toQPointF();
    QPointF ptTC = m_pDrawingScene->convert(m_physValRectScaledAndRotated.topCenter(), Units.Length.px).toQPointF();
    QPointF ptRC = m_pDrawingScene->convert(m_physValRectScaledAndRotated.rightCenter(), Units.Length.px).toQPointF();
    QPointF ptBC = m_pDrawingScene->convert(m_physValRectScaledAndRotated.bottomCenter(), Units.Length.px).toQPointF();
    QPointF ptLC = m_pDrawingScene->convert(m_physValRectScaledAndRotated.leftCenter(), Units.Length.px).toQPointF();
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
QString CPhysValPolygon::toString(bool i_bAddUnit, const QString& i_strSeparator, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QString str = "[" + QString::number(m_polygonScaledAndRotated.size()) + "]";
    if (!m_polygonScaledAndRotated.isEmpty()) {
        str += "(";
        if (i_iPrecision < 0) {
            for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygonScaledAndRotated[idxPt], i_strSeparator) + "}";
            }
        }
        else {
            for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
                if (!str.endsWith("(")) str += i_strSeparator;
                str += QString::number(idxPt) + ": {" + qPoint2Str(m_polygonScaledAndRotated[idxPt], i_strSeparator, 'f', i_iPrecision) + "}";
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
CPhysValRect CPhysValPolygon::physValBoundingRect() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated;
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::center() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.center();
}

//------------------------------------------------------------------------------
CPhysValSize CPhysValPolygon::size() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.size();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolygon::width() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.width();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolygon::height() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.height();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPolygon::angle() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.angle();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::topLeft() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.topLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::topRight() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.topRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::bottomRight() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.bottomRight();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::bottomLeft() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.bottomLeft();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::topCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.topCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::rightCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.rightCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::bottomCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.bottomCenter();
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValPolygon::leftCenter() const
//------------------------------------------------------------------------------
{
    return m_physValRectScaledAndRotated.leftCenter();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the polygon.

    The point must be passed in the current unit of the polygon.

    Invokes setCenter with the current unit.

    @param [in] i_pt
        New center point of the polygon's bounding rectangle.
*/
void CPhysValPolygon::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the polygon.

    To move a polygon the center point of the bounding rectangle is moved.
    The size and rotation angle of the bounding rectangle remain the same.
    The bounding rectangle's edges are implicitly changed.
    Each polygon point will be moved by the same vector as the center point.

    @param [in] i_physValPoint
        New center point of the polygon's bounding rectangle.
*/
void CPhysValPolygon::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setCenter(i_physValPoint);
    QPointF ptCenterModified = m_physValRectScaledAndRotated.center().toQPointF();
    QPointF ptCenterMove = ptCenterModified - m_ptCenterOrig;
    m_ptCenterOrig = ptCenterModified;
    for (int idxPt = 0; idxPt < m_polygonOrig.size(); ++idxPt) {
        m_polygonOrig[idxPt] += ptCenterMove;
    }
    for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
        m_polygonScaledAndRotated[idxPt] += ptCenterMove;
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the polygon.

    The size must be passed in the current unit of the polygon.

    Invokes setSize with the current unit.

    @param [in] i_size
        New size of the polygon's bounding rectangle.
*/
void CPhysValPolygon::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    setSize(CPhysValSize(*m_pDrawingScene, i_size, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the polygon.

    On changing the size the rotation angle of the polygons bounding rectangle
    remains the same.

    If the Y axis scale orientation is from top to bottom also the top left
    corner remains unchanged. For scale orientation from bottom to top the
    bottom left corner remains unchanged.

    To keep either the top left or bottom left corner the center point will be moved.

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

    As the polygon may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    Each polygon point must be moved by the resize scale factor of the bounding rectangle.

    @param [in] i_physValSize
        New size of the polygon's bounding rectangle.
*/
void CPhysValPolygon::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setSize(i_physValSize);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon.

    The value must be passed in the current unit of the rectangle.

*/
void CPhysValPolygon::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    setWidth(CPhysVal(i_fWidth, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon.

    The left, top and bottom edge and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edge the center point will be moved.

    The rectangle's right edge is implicitly changed.

    Width must be greater or equal than 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setWidth(i_physValWidth);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon by moving the left center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the left center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (right center)
    as well as the top right and bottom right corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setWidthByMovingLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingLeftCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon by moving the left center point.

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
void CPhysValPolygon::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setWidthByMovingLeftCenter(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon by moving the right center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the right center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (left center)
    as well as the top left and bottom left corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setWidthByMovingRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingRightCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the polygon by moving the right center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the right center selection point with mouse events.

    TODO

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setWidthByMovingRightCenter(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the polygon.

    The value must be passed in the current unit of the rectangle.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.

    The rectangle's bottom edge is implicitly changed.

    Height must be greater than or equal to 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    setHeight(CPhysVal(i_fHeight, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the polygon.

    The left and right edges as well as the rotation angle of the rectangle remains unchanged.

    If the Y axis scale orientation is from top to bottom also the top edge remains unchanged.
    If the Y axis scale orientation is from bottom to top the bottom edge remains unchanged.

    To keep the unchanged edges the center point will be moved.

    Height must be greater than or equal to 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setHeight(i_physValHeight);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the polygon by moving the top center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the top center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setHeightByMovingTopCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setHeightByMovingTopCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the polygon by moving the top center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the top center selection point with mouse events.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setHeightByMovingTopCenter(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the polygon by moving the bottom center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the bottom center selection point with mouse events.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (top center)
    as well as the top left and top right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setHeightByMovingBottomCenter(const QPointF& i_pt)
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
void CPhysValPolygon::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setHeightByMovingBottomCenter(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the polygon in degrees.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise with 0° at 3 o'clock (90° at 6 o'clock).

    @param [in] i_physValAngle
        Angle to be set.
*/
void CPhysValPolygon::setAngle( double i_fAngle_degree )
//------------------------------------------------------------------------------
{
    setAngle(CPhysVal(i_fAngle_degree, Units.Angle.Degree, 0.1));
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the polygon in the given unit.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise with 0° at 3 o'clock (90° at 6 o'clock).

    @param [in] i_physValAngle
        Angle to be set. The angle will be normalized to fit into the range 0 .. 360°.
*/
void CPhysValPolygon::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setAngle(i_physValAngle);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the top left corner of the polygon.

    The point must be passed in the current unit of the polygon.

    The rotation angle and the opposite corner (bottom right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setTopLeft(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the top left corner of the polygon.

    The rotation angle and the opposite corner (bottom right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setTopLeft(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the top right corner of the polygon.

    The point must be passed in the current unit of the polygon.

    The rotation angle and the opposite corner (bottom left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setTopRight(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the top right corner of the polygon.

    The rotation angle and the opposite corner (bottom left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setTopRight(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom right corner of the polygon.

    The point must be passed in the current unit of the polygon.

    The rotation angle and the opposite corner (top left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setBottomRight(const QPointF& i_pt)
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
void CPhysValPolygon::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setBottomRight(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom left corner of the polygon.

    The point must be passed in the current unit of the polygon.

    The rotation angle and the opposite corner (top right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setBottomLeft(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom left corner of the polygon.

    The rotation angle and the opposite corner (top right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValPolygon::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_physValRectScaledAndRotated.setBottomLeft(i_physValPoint);
    updateScaledAndRotatedPolygon();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns true, if a line should be drawn from the last point to the
           first point of the polygon.
*/
//------------------------------------------------------------------------------
bool CPhysValPolygon::isClosedPolygon() const
{
    return m_bIsClosedPolygon;
}

//------------------------------------------------------------------------------
/*! @brief Returns true, if the polygon is empty.
*/
bool CPhysValPolygon::isEmpty() const
//------------------------------------------------------------------------------
{
    return m_polygonScaledAndRotated.isEmpty();
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of polygon points.

    Cannot be named "size" as size is already used by base class CPhysValRect
    returning the size (width and height) of the rectangle.
*/
int CPhysValPolygon::count() const
//------------------------------------------------------------------------------
{
    return m_polygonScaledAndRotated.size();
}

//------------------------------------------------------------------------------
/*! @brief Returns the point at index position as a copy.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.

    @param [in] i_idx
        The index must be a valid index position in the range 0 <= i_idx < count().
*/
CPhysValPoint CPhysValPolygon::at(int i_idx) const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_polygonScaledAndRotated[i_idx]);
}

//------------------------------------------------------------------------------
/*! @brief Returns the first point of the polygon as a copy.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.
*/
CPhysValPoint CPhysValPolygon::first() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_polygonScaledAndRotated.first());
}

//------------------------------------------------------------------------------
/*! @brief Returns the last point of the polygon as a copy.

    @note An operator or method returning a modifiable reference is not provided
          as on modifying the point, some further calculations as updating the
          bounding rectangle must be triggered.
*/
CPhysValPoint CPhysValPolygon::last() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_polygonScaledAndRotated.last());
}

//------------------------------------------------------------------------------
/*! @brief Replaces the point at the given index.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_idx
        Index of the point to be replaced.
    @param [in] i_physValPoint
        Point to be set.
*/
void CPhysValPolygon::replace(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.replace(i_idx, std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Appends the point at the end of the polygon.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_physValPoint
        Point to be added.
*/
void CPhysValPolygon::append(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.append(std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Inserts the point at the given index to the polygon.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_idx
        Index of the point to be replaced.
    @param [in] i_physValPoint
        Point to be inserted.
*/
void CPhysValPolygon::insert(int i_idx, const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.insert(i_idx, std::move(i_physValPoint.toQPointF(m_unit)));
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the given number of points from the polygon starting at the given index.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_idx
        Index where the remove starts.
    @param [in] i_iCount
        Number of points to be removed.
*/
void CPhysValPolygon::remove(int i_idx, int i_iCount)
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.remove(i_idx, i_iCount);
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the point from the polygon at the given index.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_idx
        Index to be removed.
*/
void CPhysValPolygon::removeAt(int i_idx)
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.removeAt(i_idx);
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.
*/
void CPhysValPolygon::removeFirst()
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.removeFirst();
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the last point from the polygon.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.
*/
void CPhysValPolygon::removeLast()
//------------------------------------------------------------------------------
{
    m_polygonScaledAndRotated.removeLast();
    updateOriginalPolygon();
}

//------------------------------------------------------------------------------
/*! @brief Removes the point from the polygon at the given index and returns
           the removed point.

    When moving a point of the polygon, the rotated rectangle around the polygon
    may change and therefore the size and center point of the rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0 but keeping the rotation angle.

    @param [in] i_idx
        Index to be removed.
*/
CPhysValPoint CPhysValPolygon::takeAt(int i_idx)
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonScaledAndRotated.takeAt(i_idx);
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon and returns the removed point.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
CPhysValPoint CPhysValPolygon::takeFirst()
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonScaledAndRotated.takeFirst();
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Removes the first point from the polygon and returns the removed point.

    When removing a point a point from the polygon, the bounding rectangle must be updated.
    The modified polygon points become the original polygon points by the means that
    the X and Y scale factors are reset to 1.0. But the rotation angle will be kept.
*/
CPhysValPoint CPhysValPolygon::takeLast()
//------------------------------------------------------------------------------
{
    QPointF pt = m_polygonScaledAndRotated.takeLast();
    updateOriginalPolygon();
    return CPhysValPoint(*m_pDrawingScene, pt, m_unit);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the polygon as a QPolygonF instance in the current unit.
*/
QPolygonF CPhysValPolygon::toQPolygonF() const
//------------------------------------------------------------------------------
{
    return m_polygonScaledAndRotated;
}

//------------------------------------------------------------------------------
/*! @brief Returns the polygon as a QPolygonF instance in the desired unit.
*/
QPolygonF CPhysValPolygon::toQPolygonF(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPolygonF polygon = m_polygonScaledAndRotated;
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
void CPhysValPolygon::updateScaledAndRotatedPolygon()
//------------------------------------------------------------------------------
{
    QSizeF sizeModified = m_physValRectScaledAndRotated.size().toQSizeF();
    QPointF ptCenterModified = m_physValRectScaledAndRotated.center().toQPointF();
    double fXScaleFactor = 1.0;
    double fYScaleFactor = 1.0;
    // Width and height might be zero.
    if (sizeModified.width() != m_sizeOrig.width() && m_sizeOrig.width() > 0.0) {
        fXScaleFactor = sizeModified.width() / m_sizeOrig.width();
    }
    if (sizeModified.height() != m_sizeOrig.height() && m_sizeOrig.height() > 0.0) {
        fYScaleFactor = sizeModified.height() / m_sizeOrig.height();
    }
    for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
        double fdxOrig = m_polygonOrig[idxPt].x() - m_ptCenterOrig.x();
        double fdyOrig = m_polygonOrig[idxPt].y() - m_ptCenterOrig.y();
        double fdxModified = fXScaleFactor * fdxOrig;
        double fdyModified = fYScaleFactor * fdyOrig;
        m_polygonScaledAndRotated[idxPt] = QPointF(ptCenterModified.x() + fdxModified, ptCenterModified.y() + fdyModified);
        if (m_physValRectScaledAndRotated.angle().getVal() != 0.0) {
            double fAngle_rad = m_physValRectScaledAndRotated.angle().getVal(Units.Angle.Rad); // clockwise, 0° at 3 o'clock
            fAngle_rad = ZS::System::Math::toCounterClockWiseAngleRad(fAngle_rad);
            if (!m_bYAxisTopDown) {
                // For BottomUp YAxis scale reflection on the x-axis:
                fAngle_rad = Math::c_f360Degrees_rad - fAngle_rad;
            }
            m_polygonScaledAndRotated[idxPt] = ZS::Draw::rotatePoint(ptCenterModified, m_polygonScaledAndRotated[idxPt], fAngle_rad);
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Internal method to update the original polygon coordinates.

    When changing the polygon directly by either modifying points, inserting
    or removing points, the rotated rectangle around the polygon may change and
    therefore the size and center point of the rectangle must be updated.

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
void CPhysValPolygon::updateOriginalPolygon()
//------------------------------------------------------------------------------
{
    // Points may have been added or removed. The modified polygon may have a
    // different size. Just assign the modified to the original coordinates
    // to correct the sizes. For a not rotated polygon thats even the result.
    m_polygonOrig = m_polygonScaledAndRotated;
    double fAngle_rad = 0.0;
    CPhysVal physValAngle = m_physValRectScaledAndRotated.angle();
    if (physValAngle.getVal() != 0.0) {
        QPointF ptCenterModified = m_physValRectScaledAndRotated.center().toQPointF();
        fAngle_rad = physValAngle.getVal(Units.Angle.Rad); // clockwise, 0° at 3 o'clock
        fAngle_rad = ZS::System::Math::toCounterClockWiseAngleRad(fAngle_rad);
        if (!m_bYAxisTopDown) {
            // For BottomUp YAxis scale reflection on the x-axis:
            fAngle_rad = Math::c_f360Degrees_rad - fAngle_rad;
        }
        // Rotate back:
        for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
            m_polygonOrig[idxPt] = ZS::Draw::rotatePoint(ptCenterModified, m_polygonScaledAndRotated[idxPt], -fAngle_rad);
        }
    }
    m_physValRectScaledAndRotated = CPhysValRect(*m_pDrawingScene, m_unit);
    m_physValRectScaledAndRotated.setSize(CPhysValSize(*m_pDrawingScene, m_polygonOrig.boundingRect().size(), m_unit));
    m_physValRectScaledAndRotated.setCenter(CPhysValPoint(*m_pDrawingScene, m_polygonOrig.boundingRect().center(), m_unit));
    // Calculate the offset error by rotating one of the original points
    // around the current center of the bounding rectangle.
    if (!m_polygonOrig.isEmpty() && physValAngle.getVal() != 0.0) {
        QPointF ptCenter = m_physValRectScaledAndRotated.center().toQPointF();
        QPointF ptRotated = ZS::Draw::rotatePoint(ptCenter, m_polygonOrig[0], fAngle_rad);
        QPointF ptOffset = ptRotated - m_polygonScaledAndRotated[0];
        ptCenter -= ptOffset;
        m_physValRectScaledAndRotated.setCenter(ptCenter);
        for (int idxPt = 0; idxPt < m_polygonScaledAndRotated.size(); ++idxPt) {
            m_polygonOrig[idxPt] -= ptOffset;
        }
    }
    m_physValRectScaledAndRotated.setAngle(physValAngle);
    m_ptCenterOrig = m_physValRectScaledAndRotated.center().toQPointF();
    m_sizeOrig = m_physValRectScaledAndRotated.size().toQSizeF();
}
