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

#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValRect
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical rectangle on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(),
    m_size(),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_ptTL, i_ptBR).center()),
    m_size(QRectF(i_ptTL, i_ptBR).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_ptTL, i_ptBR).center()),
    m_size(QRectF(i_ptTL, i_ptBR).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValPoint& i_physValBottomRight) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_physValTopLeft.toQPointF(), i_physValBottomRight.toQPointF()).center()),
    m_size(QRectF(i_physValTopLeft.toQPointF(), i_physValBottomRight.toQPointF()).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_physValTopLeft.unit())
{
    if (i_physValTopLeft.unit() != i_physValBottomRight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValBottomRight.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValSize& i_physValSize) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_physValTopLeft.toQPointF(), i_physValSize.toQSizeF()).center()),
    m_size(QRectF(i_physValTopLeft.toQPointF(), i_physValSize.toQSizeF()).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_physValTopLeft.unit())
{
    if (i_physValTopLeft.unit() != i_physValSize.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValSize.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    m_pDrawingScene(i_physValRectOther.m_pDrawingScene),
    m_ptCenter(i_physValRectOther.m_ptCenter),
    m_size(i_physValRectOther.m_size),
    m_physValAngle(i_physValRectOther.m_physValAngle),
    m_unit(i_physValRectOther.m_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    //m_ptCenter;
    //m_size
    //m_unit;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const CPhysValRect& i_physValRectOther )
//------------------------------------------------------------------------------
{
    m_ptCenter = i_physValRectOther.m_ptCenter;
    m_size = i_physValRectOther.m_size;
    m_physValAngle = i_physValRectOther.m_physValAngle;
    m_unit = i_physValRectOther.m_unit;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    m_ptCenter = i_rect.center();
    m_size = i_rect.size();
    m_physValAngle = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRect::operator == ( const CPhysValRect& i_physValRectOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (!areOfSameUnitGroup(m_unit, i_physValRectOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit != i_physValRectOther.m_unit) {
        bEqual = false;
    }
    else if (m_ptCenter != i_physValRectOther.m_ptCenter) {
        bEqual = false;
    }
    else if (m_size != i_physValRectOther.m_size) {
        bEqual = false;
    }
    else if (m_physValAngle != i_physValRectOther.m_physValAngle) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValRect::operator != ( const CPhysValRect& i_physValRectOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValRectOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is valid, otherwise returns false.

    A valid rectangle has a width() > 0 and height() > 0.
*/
bool CPhysValRect::isValid() const
//------------------------------------------------------------------------------
{
    return m_size.isValid();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the center point of the rectangle.
*/
CPhysValPoint CPhysValRect::center() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_ptCenter, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the size (width and height) of the rectangle.

    If the left edge of the unrotated rectangle is right of the right edge,
    the returned width is negative.
    If the top edge of the unrotated rectangle is below the bottom edge,
    the returned height is negative.
*/
CPhysValSize CPhysValRect::size() const
//------------------------------------------------------------------------------
{
    return CPhysValSize(*m_pDrawingScene, m_size, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the width of the rectangle.

    If the left edge of the unrotated rectangle is right of the right edge,
    the returned width is negative.
*/
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_size.width(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
/*! @brief Returns the height of the rectangle.

    If the top edge of the unrotated rectangle is below the bottom edge,
    the returned height is negative.
*/
CPhysVal CPhysValRect::height() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_size.height(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
/*! @brief Returns the rotation angle of the rectangle in degrees.

    The angles are measured clockwise around the z-Axis through the center point.
*/
CPhysVal CPhysValRect::angle() const
//------------------------------------------------------------------------------
{
    return m_physValAngle;
}

//------------------------------------------------------------------------------
/*! @brief Returns the top left corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    if (fAngle_rad == 0.0) {
        fX -= m_size.width() / 2.0;
        fY -= m_size.height() / 2.0;
    }
    else {
        fX -= radius(m_size) * cos(phi_rad(m_size) + fAngle_rad);
        fY -= radius(m_size) * sin(phi_rad(m_size) + fAngle_rad);
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the top right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    if (fAngle_rad == 0.0) {
        fX += m_size.width() / 2.0;
        fY -= m_size.height() / 2.0;
    }
    else {
        fX += radius(m_size) * cos(phi_rad(m_size) + fAngle_rad);
        fY -= radius(m_size) * sin(phi_rad(m_size) + fAngle_rad);
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    if (fAngle_rad == 0.0) {
        fX += m_size.width() / 2.0;
        fY += m_size.height() / 2.0;
    }
    else {
        fX += radius(m_size) * cos(phi_rad(m_size) + fAngle_rad);
        fY += radius(m_size) * sin(phi_rad(m_size) + fAngle_rad);
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom left corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    if (fAngle_rad == 0.0) {
        fX -= m_size.width() / 2.0;
        fY += m_size.height() / 2.0;
    }
    else {
        fX -= radius(m_size) * cos(phi_rad(m_size) + fAngle_rad);
        fY += radius(m_size) * sin(phi_rad(m_size) + fAngle_rad);
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the top center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::topCenter() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    if (m_physValAngle.getVal() == 0.0) {
        fY -= m_size.height() / 2.0;
    }
    else {
        CPhysValPoint physValPtTL = topLeft();
        CPhysValPoint physValPtTR = topRight();
        fX = (physValPtTL.x().getVal() + physValPtTR.x().getVal()) / 2.0;
        fY = (physValPtTL.y().getVal() + physValPtTR.y().getVal()) / 2.0;
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the right center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::rightCenter() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    if (m_physValAngle.getVal() == 0.0) {
        fX += m_size.width() / 2.0;
    }
    else {
        CPhysValPoint physValPtTR = topRight();
        CPhysValPoint physValPtBR = bottomRight();
        fX = (physValPtTR.x().getVal() + physValPtBR.x().getVal()) / 2.0;
        fY = (physValPtTR.y().getVal() + physValPtBR.y().getVal()) / 2.0;
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::bottomCenter() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    if (m_physValAngle.getVal() == 0.0) {
        fY += m_size.height() / 2.0;
    }
    else {
        CPhysValPoint physValPtBR = bottomRight();
        CPhysValPoint physValPtBL = bottomLeft();
        fX = (physValPtBR.x().getVal() + physValPtBL.x().getVal()) / 2.0;
        fY = (physValPtBR.y().getVal() + physValPtBL.y().getVal()) / 2.0;
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the left center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and phi_rad.
*/
CPhysValPoint CPhysValRect::leftCenter() const
//------------------------------------------------------------------------------
{
    double fX = m_ptCenter.x();
    double fY = m_ptCenter.y();
    if (m_physValAngle.getVal() == 0.0) {
        fX -= m_size.width() / 2.0;
    }
    else {
        CPhysValPoint physValPtTL = topLeft();
        CPhysValPoint physValPtBL = bottomLeft();
        fX = (physValPtTL.x().getVal() + physValPtBL.x().getVal()) / 2.0;
        fY = (physValPtTL.y().getVal() + physValPtBL.y().getVal()) / 2.0;
    }
    return CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the rectangle coordinates.

    The resolution will be retrieved from the drawing scene's drawing size and
    depends on the current unit.
*/
double CPhysValRect::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
}

//------------------------------------------------------------------------------
/*! @brief Returns the current unit the coordinates are stored.
*/
CUnit CPhysValRect::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The size and rotation angle of the rectangle remain the same.
    The rectangle's edges are implicitly changed.
*/
void CPhysValRect::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_ptCenter = i_pt;
}

//------------------------------------------------------------------------------
/*! @brief Sets the center point of the rectangle.

    The size and rotation angle of the rectangle remain the same.
    The rectangle's edges are implicitly changed.
*/
void CPhysValRect::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    m_ptCenter = i_physValPoint.toQPointF(m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the rectangle.

    The size must be passed in the current unit of the rectangle.

    The top left corner and the rotation angle of the rectangle remains the same.
    To keep the top left corner the center point will be moved.

    The rectangle's right and bottom edges are implicitly changed.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    setSize(CPhysValSize(*m_pDrawingScene, i_size, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the size (width and height) of the rectangle.

    The top left corner and the rotation angle of the rectangle remains the same.
    To keep the top left corner the center point will be moved.

    The rectangle's right and bottom edges are implicitly changed.

    If width is negative, the left edge is right of the right edge.
    If height is negative, the top edge is below the bottom edge.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal();
    double fY = physValPtTL.y().getVal();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    QSizeF sizeF = i_physValSize.toQSizeF(m_unit);
    if (fAngle_rad == 0.0) {
        fX += sizeF.width() / 2.0;
        fY += sizeF.height() / 2.0;
    }
    else {
        fX += radius(sizeF) * cos(phi_rad(sizeF) + fAngle_rad);
        fY += radius(sizeF) * sin(phi_rad(sizeF) + fAngle_rad);
    }
    m_ptCenter = QPointF(fX, fY);
    m_size = sizeF;
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The left, top and bottom edges and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edges the center point will be moved.

    The rectangle's right edge is implicitly changed.

    If width is negative, the left edge is right of the right edge.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    setWidth(CPhysVal(i_fWidth, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle.

    The left, top and bottom edge and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edge the center point will be moved.

    The rectangle's right edge is implicitly changed.

    If width is negative, the left edge is right of the right edge.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal();
    double fY = physValPtTL.y().getVal();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    CPhysVal physValHeight = height();
    QSizeF sizeF(i_physValWidth.getVal(m_unit), physValHeight.getVal());
    if (fAngle_rad == 0.0) {
        fX += sizeF.width() / 2.0;
        fY += sizeF.height() / 2.0;
    }
    else {
        fX += radius(sizeF) * cos(phi_rad(sizeF) + fAngle_rad);
        fY += radius(sizeF) * sin(phi_rad(sizeF) + fAngle_rad);
    }
    m_ptCenter = QPointF(fX, fY);
    m_size = sizeF;
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
void CPhysValRect::setWidthByMovingLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingLeftCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the left center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the left center selection point with mouse events.

    The rotation angle, the height and the opposite center point (right center)
    as well as the top right and bottom right corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QRectF rectFNotRotated = toNotRotatedQRectF();
    QPointF ptNew = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() != 0.0) {
        // Rotate the given point around the center to fit the unrotated rectangle.
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        ptNew = ZS::Draw::rotatePoint(m_ptCenter, ptNew, -fAngle_rad);
    }
    rectFNotRotated.setLeft(ptNew.x());
    m_size = rectFNotRotated.size();
    m_ptCenter = rectFNotRotated.center();
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
void CPhysValRect::setWidthByMovingRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setWidthByMovingRightCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle by moving the right center point.

    This method is mainly provided to allow resizing a rectangle object on the
    graphics scene by moving the right center selection point with mouse events.

    The rotation angle, the height and the opposite center point (left center)
    as well as the top left and bottom left corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QRectF rectFNotRotated = toNotRotatedQRectF();
    QPointF ptNew = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() != 0.0) {
        // Rotate the given point around the center to fit the unrotated rectangle.
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        ptNew = ZS::Draw::rotatePoint(m_ptCenter, ptNew, -fAngle_rad);
    }
    rectFNotRotated.setRight(ptNew.x());
    m_size = rectFNotRotated.size();
    m_ptCenter = rectFNotRotated.center();
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.

    The rectangle's bottom edge is implicitly changed.

    If height is negative, the top edge is below the bottom edge.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    setHeight(CPhysVal(i_fHeight, m_unit, m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.
    If the unit is changed the center point will be converted correspondingly.

    The rectangle's bottom edge is implicitly changed.

    If height is negative, the top edge is below the bottom edge.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal();
    double fY = physValPtTL.y().getVal();
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    CPhysVal physValWidth = width();
    QSizeF sizeF(physValWidth.getVal(m_unit), i_physValHeight.getVal());
    if (fAngle_rad == 0.0) {
        fX += sizeF.width() / 2.0;
        fY += sizeF.height() / 2.0;
    }
    else {
        fX += radius(sizeF) * cos(phi_rad(sizeF) + fAngle_rad);
        fY += radius(sizeF) * sin(phi_rad(sizeF) + fAngle_rad);
    }
    m_ptCenter = QPointF(fX, fY);
    m_size = sizeF;
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
void CPhysValRect::setHeightByMovingTopCenter(const QPointF& i_pt)
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
void CPhysValRect::setHeightByMovingTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QRectF rectFNotRotated = toNotRotatedQRectF();
    QPointF ptNew = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() != 0.0) {
        // Rotate the given point around the center to fit the unrotated rectangle.
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        ptNew = ZS::Draw::rotatePoint(m_ptCenter, ptNew, -fAngle_rad);
    }
    rectFNotRotated.setTop(ptNew.y());
    m_size = rectFNotRotated.size();
    m_ptCenter = rectFNotRotated.center();
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
void CPhysValRect::setHeightByMovingBottomCenter(const QPointF& i_pt)
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
void CPhysValRect::setHeightByMovingBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QRectF rectFNotRotated = toNotRotatedQRectF();
    QPointF ptNew = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() != 0.0) {
        // Rotate the given point around the center to fit the unrotated rectangle.
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        ptNew = ZS::Draw::rotatePoint(m_ptCenter, ptNew, -fAngle_rad);
    }
    rectFNotRotated.setBottom(ptNew.y());
    m_size = rectFNotRotated.size();
    m_ptCenter = rectFNotRotated.center();
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the rectangle in the given unit.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise around the z-Axis through the center
    point of the rectangle.

    @param [in] i_physValAngle
        Angle to be set.
*/
void CPhysValRect::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    m_physValAngle = i_physValAngle;
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
void CPhysValRect::setTopLeft(const QPointF& i_pt)
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
void CPhysValRect::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_physValPoint.unit(), i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValPoint physValPtOpposite = bottomRight();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal(m_unit) + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal(m_unit) + physValPtOpposite.y().getVal()) / 2.0));
    // Diagonale from TopLeft to BottomRight:
    // - TopLeft.x may be right of BottomRight.x (dx < 0) -> Width is negative
    // - TopLeft.y may be below BottomRight.y (dy < 0)  -> Height is negative
    QLineF lineDiagonale(i_physValPoint.toQPointF(), physValPtOpposite.toQPointF());
    double fDiagonale = lineDiagonale.length();
    double fWidth = 0.0;
    double fHeight = 0.0;
    if (fDiagonale > 0.0) {
        double dx = lineDiagonale.dx();
        double dy = lineDiagonale.dy();
        fWidth = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dy));
        fHeight = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dx));
        if (dx < 0.0) {
            fWidth *= -1.0;
        }
        if (dy < 0.0) {
            fHeight *= -1.0;
        }
    }
    m_size.setWidth(fWidth);
    m_size.setHeight(fHeight);
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
void CPhysValRect::setTopRight(const QPointF& i_pt)
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
void CPhysValRect::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_physValPoint.unit(), i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValPoint physValPtOpposite = bottomLeft();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal(m_unit) + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal(m_unit) + physValPtOpposite.y().getVal()) / 2.0));
    // Diagonale from TopRight to BottomLeft:
    // - TopRight.x may be left of BottomLeft.x (dx > 0) -> Width is negative
    // - TopRight.y may be below BottomLeft.y (dy < 0) -> Height is negative
    QLineF lineDiagonale(i_physValPoint.toQPointF(), physValPtOpposite.toQPointF());
    double fDiagonale = lineDiagonale.length();
    double fWidth = 0.0;
    double fHeight = 0.0;
    if (fDiagonale > 0.0) {
        double dx = lineDiagonale.dx();
        double dy = lineDiagonale.dy();
        fWidth = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dy));
        fHeight = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dx));
        if (dx > 0.0) {
            fWidth *= -1.0;
        }
        if (dy < 0.0) {
            fHeight *= -1.0;
        }
    }
    m_size.setWidth(fWidth);
    m_size.setHeight(fHeight);
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
void CPhysValRect::setBottomRight(const QPointF& i_pt)
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
void CPhysValRect::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_physValPoint.unit(), i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValPoint physValPtOpposite = topLeft();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal(m_unit) + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal(m_unit) + physValPtOpposite.y().getVal()) / 2.0));
    // Diagonale from BottomRight to TopLeft:
    // - BottomRight.x may be left of TopLeft.x (dx > 0) -> Width is negative
    // - BottomRight.y may be above TopLeft.y (dy > 0) -> Height is negative
    QLineF lineDiagonale(i_physValPoint.toQPointF(), physValPtOpposite.toQPointF());
    double fDiagonale = lineDiagonale.length();
    double fWidth = 0.0;
    double fHeight = 0.0;
    if (fDiagonale > 0.0) {
        double dx = lineDiagonale.dx();
        double dy = lineDiagonale.dy();
        fWidth = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dy));
        fHeight = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dx));
        if (dx > 0.0) {
            fWidth *= -1.0;
        }
        if (dy > 0.0) {
            fHeight *= -1.0;
        }
    }
    m_size.setWidth(fWidth);
    m_size.setHeight(fHeight);
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
void CPhysValRect::setBottomLeft(const QPointF& i_pt)
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
void CPhysValRect::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_physValPoint.unit(), i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValPoint physValPtOpposite = topRight();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal(m_unit) + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal(m_unit) + physValPtOpposite.y().getVal()) / 2.0));
    // Diagonale from BottomLeft to TopTight:
    // - BottomLeft.x may be right of TopTight.x (dx < 0) -> Width is negative
    // - BottomLeft.y may be above TopTight.y (dy > 0) -> Height is negative
    QLineF lineDiagonale(i_physValPoint.toQPointF(), physValPtOpposite.toQPointF());
    double fDiagonale = lineDiagonale.length();
    double fWidth = 0.0;
    double fHeight = 0.0;
    if (fDiagonale > 0.0) {
        double dx = lineDiagonale.dx();
        double dy = lineDiagonale.dy();
        fWidth = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dy));
        fHeight = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(dx));
        if (dx < 0.0) {
            fWidth *= -1.0;
        }
        if (dy > 0.0) {
            fHeight *= -1.0;
        }
    }
    m_size.setWidth(fWidth);
    m_size.setHeight(fHeight);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical rectangle as a QRectF instance of the !!unrotated!!
           rectangle in the current unit of the physical rectangle.
*/
QRectF CPhysValRect::toNotRotatedQRectF() const
//------------------------------------------------------------------------------
{
    QPointF ptTopLeft(m_ptCenter.x() - m_size.width()/2.0, m_ptCenter.y() - m_size.height()/2.0);
    return QRectF(ptTopLeft, m_size);
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical point as a QPointF instance in the desired unit.
* 
*/
QRectF CPhysValRect::toNotRotatedQRectF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPointF ptTopLeft(m_ptCenter.x() - m_size.width()/2.0, m_ptCenter.y() - m_size.height()/2.0);
    QSizeF sizeF = m_size;
    if (i_unit != m_unit) {
        ptTopLeft.setX(CPhysVal(ptTopLeft.x(), m_unit).getVal(i_unit));
        ptTopLeft.setY(CPhysVal(ptTopLeft.y(), m_unit).getVal(i_unit));
        sizeF.setWidth(CPhysVal(m_size.width(), m_unit).getVal(i_unit));
        sizeF.setHeight(CPhysVal(m_size.height(), m_unit).getVal(i_unit));
    }
    return QRectF(ptTopLeft, sizeF);
}

//------------------------------------------------------------------------------
QString CPhysValRect::toString(bool i_bAddUnit, const QString& i_strSeparator) const
//------------------------------------------------------------------------------
{
    QString str = topLeft().toString() + i_strSeparator + size().toString();
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    return str;
}

/*==============================================================================
protected: // auxiliary math functions
==============================================================================*/

//------------------------------------------------------------------------------
double CPhysValRect::radius(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    double fRadius = 0.0;
    if (i_size.isValid()) {
        fRadius = Math::sqrt(Math::sqr(i_size.width()) + Math::sqr(i_size.height()));
    }
    return fRadius;
}

//------------------------------------------------------------------------------
double CPhysValRect::phi_rad(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    double fAngle_rad = (i_size.height() > 0.0) ? Math::c_f90Degrees_rad : -Math::c_f90Degrees_rad;
    if (i_size.width() != 0.0) {
        fAngle_rad = atan(i_size.height()/i_size.width());
    }
    return fAngle_rad;
}
