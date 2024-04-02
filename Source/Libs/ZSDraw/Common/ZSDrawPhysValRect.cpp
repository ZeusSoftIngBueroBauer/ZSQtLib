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
*/
CPhysValSize CPhysValRect::size() const
//------------------------------------------------------------------------------
{
    return CPhysValSize(*m_pDrawingScene, m_size, m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the width of the rectangle.
*/
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_size.width(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
/*! @brief Returns the height of the rectangle.
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

    The unit will be taken over from the given point.

    The size and rotation angle of the rectangle remain the same but if the
    unit is changed the size will be converted correspondingly.
    The rectangle's edges are implicitly changed.
*/
void CPhysValRect::setCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_ptCenter = i_physValPoint.toQPointF();
    if (m_unit != i_physValPoint.unit()) {
        CPhysVal physValWidth(m_size.width(), m_unit);
        CPhysVal physValHeight(m_size.height(), m_unit);
        physValWidth.convertValue(i_physValPoint.unit());
        physValHeight.convertValue(i_physValPoint.unit());
        m_size.setWidth(physValWidth.getVal());
        m_size.setHeight(physValHeight.getVal());
        m_unit = i_physValPoint.unit();
    }
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

    The unit will be taken over from the given size.

    The top left corner and the rotation angle of the rectangle remains the same.
    To keep the top left corner the center point will be moved.
    If the unit is changed the center point will be converted correspondingly.

    The rectangle's right and bottom edges are implicitly changed.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal(i_physValSize.unit());
    double fY = physValPtTL.y().getVal(i_physValSize.unit());
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    QSizeF sizeF = i_physValSize.toQSizeF();
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
    m_unit = i_physValSize.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the width of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The left, top and bottom edges and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edges the center point will be moved.

    The rectangle's right edge is implicitly changed.

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

    The unit will be taken over from the given value.

    The left, top and bottom edge and the rotation angle of the rectangle remain
    the same. To keep the left, top and bottom edge the center point will be moved.
    If the unit is changed the center point will be converted correspondingly.

    The rectangle's right edge is implicitly changed.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal(i_physValWidth.unit());
    double fY = physValPtTL.y().getVal(i_physValWidth.unit());
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    CPhysVal physValHeight = height();
    physValHeight.convertValue(i_physValWidth.unit());
    QSizeF sizeF(i_physValWidth.getVal(), physValHeight.getVal());
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
    m_unit = i_physValWidth.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the height of the rectangle.

    The value must be passed in the current unit of the rectangle.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.

    The rectangle's bottom edge is implicitly changed.

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

    The unit will be taken over from the given value.

    The top, left and right edges and the rotation angle of the rectangle remain
    the same. To keep the top, left and right edges the center point will be moved.
    If the unit is changed the center point will be converted correspondingly.

    The rectangle's bottom edge is implicitly changed.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    // Before taken over the new size, get current top left corner.
    CPhysValPoint physValPtTL = topLeft();
    double fX = physValPtTL.x().getVal(i_physValHeight.unit());
    double fY = physValPtTL.y().getVal(i_physValHeight.unit());
    double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
    CPhysVal physValWidth = width();
    physValWidth.convertValue(i_physValHeight.unit());
    QSizeF sizeF(physValWidth.getVal(), i_physValHeight.getVal());
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
    m_unit = i_physValHeight.unit();
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

    The unit will be taken over from the given point.

    The rotation angle and the opposite corner (bottom right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    If the unit is changed the center point and the size will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    updateFromOppositeCorners(i_physValPoint, bottomRight());
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

    The unit will be taken over from the given point.

    The rotation angle and the opposite corner (bottom left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    If the unit is changed the center point and the size will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    updateFromOppositeCorners(i_physValPoint, bottomLeft());
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

    The unit will be taken over from the given point.

    The rotation angle and the opposite corner (top left) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    If the unit is changed the center point and the size will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    updateFromOppositeCorners(i_physValPoint, topLeft());
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

    The unit will be taken over from the given point.

    The rotation angle and the opposite corner (top right) of the rectangle
    remain the same. Therefore the size (width and height) and the center point
    of the rectangle are adjusted.

    If the unit is changed the center point and the size will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    updateFromOppositeCorners(i_physValPoint, topRight());
}

//------------------------------------------------------------------------------
/*! @brief Sets the top center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setTopCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setTopCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the top center point of the rectangle.

    The unit will be taken over from the given point.

    The rotation angle, the width and the opposite center point (bottom center)
    as well as the bottom left and bottom right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    If the unit is changed the center point and the height will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setTopCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    // Get coordinate of opposite point.
    CPhysValPoint physValPtOpposite = bottomCenter();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal() + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal() + physValPtOpposite.y().getVal()) / 2.0));
    double fDX = i_physValPoint.x().getVal() - m_ptCenter.x();
    double fDY = i_physValPoint.y().getVal() - m_ptCenter.y();
    if (fDX != 0.0 || fDY != 0.0) {
        double fHeight = 2.0 * Math::sqrt(Math::sqr(fDX) + Math::sqr(fDY));
        m_size.setHeight(fHeight);
    }
    m_unit = i_physValPoint.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the right center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (left center)
    as well as the top left and bottom left corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setRightCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setRightCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the right center point of the rectangle.

    The unit will be taken over from the given point.

    The rotation angle, the height and the opposite center point (left center)
    as well as the top left and bottom left corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    If the unit is changed the center point and the width will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    // Get coordinate of opposite point.
    CPhysValPoint physValPtOpposite = leftCenter();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal() + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal() + physValPtOpposite.y().getVal()) / 2.0));
    double fDX = i_physValPoint.x().getVal() - m_ptCenter.x();
    double fDY = i_physValPoint.y().getVal() - m_ptCenter.y();
    if (fDX != 0.0 || fDY != 0.0) {
        double fWidth = 2.0 * Math::sqrt(Math::sqr(fDX) + Math::sqr(fDY));
        m_size.setWidth(fWidth);
    }
    m_unit = i_physValPoint.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the width and the opposite center point (top center)
    as well as the top left and top right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setBottomCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setBottomCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the bottom center point of the rectangle.

    The unit will be taken over from the given point.

    The rotation angle, the width and the opposite center point (top center)
    as well as the top left and top right corners of the rectangle remain the same.
    Therefore the height and the center point of the rectangle are adjusted.

    If the unit is changed the center point and the height will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setBottomCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    // Get coordinate of opposite point.
    CPhysValPoint physValPtOpposite = topCenter();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal() + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal() + physValPtOpposite.y().getVal()) / 2.0));
    double fDX = i_physValPoint.x().getVal() - m_ptCenter.x();
    double fDY = i_physValPoint.y().getVal() - m_ptCenter.y();
    if (fDX != 0.0 || fDY != 0.0) {
        double fHeight = 2.0 * Math::sqrt(Math::sqr(fDX) + Math::sqr(fDY));
        m_size.setHeight(fHeight);
    }
    m_unit = i_physValPoint.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the left center point of the rectangle.

    The point must be passed in the current unit of the rectangle.

    The rotation angle, the height and the opposite center point (right center)
    as well as the top right and bottom right corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setLeftCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setLeftCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Sets the left center point of the rectangle.

    The unit will be taken over from the given point.

    The rotation angle, the height and the opposite center point (right center)
    as well as the top right and bottom right corners of the rectangle remain the same.
    Therefore the width and the center point of the rectangle are adjusted.

    If the unit is changed the center point and the width will be converted correspondingly.

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    // Get coordinate of opposite point.
    CPhysValPoint physValPtOpposite = rightCenter();
    m_ptCenter.setX(fabs((i_physValPoint.x().getVal() + physValPtOpposite.x().getVal()) / 2.0));
    m_ptCenter.setY(fabs((i_physValPoint.y().getVal() + physValPtOpposite.y().getVal()) / 2.0));
    double fDX = i_physValPoint.x().getVal() - m_ptCenter.x();
    double fDY = i_physValPoint.y().getVal() - m_ptCenter.y();
    if (fDX != 0.0 || fDY != 0.0) {
        double fWidth = 2.0 * Math::sqrt(Math::sqr(fDX) + Math::sqr(fDY));
        m_size.setWidth(fWidth);
    }
    m_unit = i_physValPoint.unit();
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

//------------------------------------------------------------------------------
void CPhysValRect::updateFromOppositeCorners(const CPhysValPoint& i_physValPtNew, const CPhysValPoint& i_physValPtOpposite)
//------------------------------------------------------------------------------
{
    m_ptCenter.setX(fabs((i_physValPtNew.x().getVal() + i_physValPtOpposite.x().getVal(i_physValPtNew.unit())) / 2.0));
    m_ptCenter.setY(fabs((i_physValPtNew.y().getVal() + i_physValPtOpposite.y().getVal(i_physValPtNew.unit())) / 2.0));
    QLineF lineDiagonale(i_physValPtNew.toQPointF(), i_physValPtOpposite.toQPointF());
    double fDiagonale = lineDiagonale.length();
    double fWidth = 0.0;
    double fHeight = 0.0;
    if (fDiagonale > 0.0) {
        fWidth = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(lineDiagonale.dy()));
        fHeight = Math::sqrt(Math::sqr(fDiagonale) - Math::sqr(lineDiagonale.dx()));
    }
    m_size.setWidth(fWidth);
    m_size.setHeight(fHeight);
    m_unit = i_physValPtNew.unit();
}
