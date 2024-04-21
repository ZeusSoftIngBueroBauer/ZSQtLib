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

************************f*******************************************************/

#include "ZSDraw/Common/ZSDrawPhysValRect.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysAux.h"
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
    m_unit(i_drawingScene.drawingSize().unit()),
    m_fRadius(0.0),
    m_fPhi_rad(0.0),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_ptTL, i_ptBR).center()),
    m_size(QRectF(i_ptTL, i_ptBR).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_drawingScene.drawingSize().unit()),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(QRectF(i_ptTL, i_ptBR).center()),
    m_size(QRectF(i_ptTL, i_ptBR).size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_unit),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_drawingScene.drawingSize().unit()),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.size()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_unit(i_unit),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
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
    m_unit(i_physValTopLeft.unit()),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    if (i_physValTopLeft.unit() != i_physValBottomRight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValBottomRight.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    initSelectionPoints();
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
    m_unit(i_physValTopLeft.unit()),
    m_fRadius(radius(m_size)),
    m_fPhi_rad(fabs(angle_rad(m_size))),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    if (i_physValTopLeft.unit() != i_physValSize.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValSize.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    m_pDrawingScene(i_physValRectOther.m_pDrawingScene),
    m_ptCenter(i_physValRectOther.m_ptCenter),
    m_size(i_physValRectOther.m_size),
    m_physValAngle(i_physValRectOther.m_physValAngle),
    m_unit(i_physValRectOther.m_unit),
    m_fRadius(i_physValRectOther.m_fRadius),
    m_fPhi_rad(i_physValRectOther.m_fPhi_rad),
    m_arphysValPoints(),
    m_arbPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    //m_ptCenter;
    //m_size;
    //m_physValAngle;
    //m_unit;
    m_fRadius = 0.0;
    m_fPhi_rad = 0.0;
    //m_arphysValPoints;
    //m_arbPointsCalculated;
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
    m_fRadius = i_physValRectOther.m_fRadius;
    m_fPhi_rad = i_physValRectOther.m_fPhi_rad;
    m_arphysValPoints = i_physValRectOther.m_arphysValPoints;
    m_arbPointsCalculated = i_physValRectOther.m_arbPointsCalculated;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    m_ptCenter = i_rect.center();
    m_size = i_rect.size();
    m_physValAngle = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    initSelectionPoints();
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

    A valid rectangle has both width and height greater than 0.0.
*/
bool CPhysValRect::isValid() const
//------------------------------------------------------------------------------
{
    // QSizeF::isValid cannot be used as the method returns false if either width
    // or height is less than 0.0. For metric units with YAxisScaleOrientation
    // BottomUp the top edge has lower values than the bottom edge and the height
    // is negative. Also negative widths are allowed.
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

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    #pragma message(__TODO__"Implement class Unit pixel converter derived from QObject instantiated in drawing scene and groups")
    #pragma message(__TODO__"Add member to point to Unit pixel converter with destroyed signal instead of reference to drawing scene")
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopLeft)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        if (fAngle_rad == 0.0) {
            fX -= m_size.width() / 2.0;
            fY -= m_size.height() / 2.0;
        }
        else {
            double fPhiTL_rad = Math::c_f180Degrees_rad - m_fPhi_rad;
            fX += m_fRadius * cos(fPhiTL_rad - fAngle_rad);
            fY -= m_fRadius * sin(fPhiTL_rad - fAngle_rad);
        }
        m_arphysValPoints[static_cast<int>(ESelectionPoint::TopLeft)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopLeft)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::TopLeft)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the top right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopRight)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        if (fAngle_rad == 0.0) {
            fX += m_size.width() / 2.0;
            fY -= m_size.height() / 2.0;
        }
        else {
            double fPhiTL_rad = m_fPhi_rad;
            fX += m_fRadius * cos(fPhiTL_rad - fAngle_rad);
            fY -= m_fRadius * sin(fPhiTL_rad - fAngle_rad);
        }
        m_arphysValPoints[static_cast<int>(ESelectionPoint::TopRight)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopRight)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::TopRight)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomRight)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        if (fAngle_rad == 0.0) {
            fX += m_size.width() / 2.0;
            fY += m_size.height() / 2.0;
        }
        else {
            double fPhiTL_rad = Math::c_f360Degrees_rad - m_fPhi_rad;
            fX += m_fRadius * cos(fPhiTL_rad - fAngle_rad);
            fY -= m_fRadius * sin(fPhiTL_rad - fAngle_rad);
        }
        m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomRight)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomRight)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomRight)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom left corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomLeft)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        double fAngle_rad = m_physValAngle.getVal(Units.Angle.Rad);
        if (fAngle_rad == 0.0) {
            fX -= m_size.width() / 2.0;
            fY += m_size.height() / 2.0;
        }
        else {
            double fPhiTL_rad = Math::c_f180Degrees_rad + m_fPhi_rad;
            fX += m_fRadius * cos(fPhiTL_rad - fAngle_rad);
            fY -= m_fRadius * sin(fPhiTL_rad - fAngle_rad);
        }
        m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomLeft)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomLeft)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomLeft)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the top center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::topCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopCenter)]) {
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
        m_arphysValPoints[static_cast<int>(ESelectionPoint::TopCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::TopCenter)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::TopCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the right center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::rightCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::RightCenter)]) {
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
        m_arphysValPoints[static_cast<int>(ESelectionPoint::RightCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::RightCenter)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::RightCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::bottomCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomCenter)]) {
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
        m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::BottomCenter)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the left center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.

    @see protected auxiliary math functions radius and angle_rad.
*/
CPhysValPoint CPhysValRect::leftCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbPointsCalculated[static_cast<int>(ESelectionPoint::LeftCenter)]) {
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
        m_arphysValPoints[static_cast<int>(ESelectionPoint::LeftCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbPointsCalculated[static_cast<int>(ESelectionPoint::LeftCenter)] = true;
    }
    return m_arphysValPoints[static_cast<int>(ESelectionPoint::LeftCenter)];
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
    invalidateSelectionPoints();
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
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValSize.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
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
        fX += radius(sizeF) * cos(angle_rad(sizeF) + fAngle_rad);
        fY += radius(sizeF) * sin(angle_rad(sizeF) + fAngle_rad);
    }
    m_ptCenter = QPointF(fX, fY);
    m_size = sizeF;
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValWidth.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setWidth(i_physValWidth.getVal(m_unit));
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QLineF lineWidth(leftCenter().toQPointF(), rightCenter().toQPointF());
        lineWidth.setLength(i_physValWidth.getVal(m_unit));
        m_ptCenter = lineWidth.center();
        m_size.setWidth(lineWidth.length());
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    functions applied to the distance (radius) of the selection point to the center point.
*/
void CPhysValRect::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setLeft(ptMoved.x());
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QPointF ptLeftCenter = leftCenter().toQPointF();
        QLineF lineWidth(rightCenter().toQPointF(), ptLeftCenter);
        QLineF linePerpendicular = getPerpendicularLine(lineWidth, ptMoved, 100.0);
        if (lineWidth.intersects(linePerpendicular, &ptLeftCenter) != QLineF::NoIntersection) {
            lineWidth.setP2(ptLeftCenter);
            m_ptCenter = lineWidth.center();
            m_size.setWidth(lineWidth.length());
            m_arphysValPoints[static_cast<int>(ESelectionPoint::LeftCenter)] =
                CPhysValPoint(*m_pDrawingScene, ptLeftCenter, m_unit);
        }
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::RightCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setRight(ptMoved.x());
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QPointF ptRightCenter = rightCenter().toQPointF();
        QLineF lineWidth(leftCenter().toQPointF(), ptRightCenter);
        QLineF linePerpendicular = getPerpendicularLine(lineWidth, ptMoved, 100.0);
        if (lineWidth.intersects(linePerpendicular, &ptRightCenter) != QLineF::NoIntersection) {
            lineWidth.setP2(ptRightCenter);
            m_ptCenter = lineWidth.center();
            m_size.setWidth(lineWidth.length());
            m_arphysValPoints[static_cast<int>(ESelectionPoint::RightCenter)] =
                CPhysValPoint(*m_pDrawingScene, ptRightCenter, m_unit);
        }
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValHeight.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setHeight(i_physValHeight.getVal(m_unit));
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QLineF lineHeight(topCenter().toQPointF(), bottomCenter().toQPointF());
        lineHeight.setLength(i_physValHeight.getVal(m_unit));
        m_ptCenter = lineHeight.center();
        m_size.setHeight(lineHeight.length());
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setTop(ptMoved.y());
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QPointF ptTopCenter = topCenter().toQPointF();
        QLineF lineHeight(bottomCenter().toQPointF(), ptTopCenter);
        QLineF linePerpendicular = getPerpendicularLine(lineHeight, ptMoved, 100.0);
        if (lineHeight.intersects(linePerpendicular, &ptTopCenter) != QLineF::NoIntersection) {
            lineHeight.setP2(ptTopCenter);
            m_ptCenter = lineHeight.center();
            m_size.setHeight(lineHeight.length());
            m_arphysValPoints[static_cast<int>(ESelectionPoint::TopCenter)] =
                CPhysValPoint(*m_pDrawingScene, ptTopCenter, m_unit);
        }
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    if (m_physValAngle.getVal() == 0.0) {
        QRectF rectFNotRotated = toNotRotatedQRectF();
        rectFNotRotated.setBottom(ptMoved.y());
        m_ptCenter = rectFNotRotated.center();
        m_size = rectFNotRotated.size();
    }
    else {
        QPointF ptBottomCenter = bottomCenter().toQPointF();
        QLineF lineHeight(topCenter().toQPointF(), ptBottomCenter);
        QLineF linePerpendicular = getPerpendicularLine(lineHeight, ptMoved, 100.0);
        if (lineHeight.intersects(linePerpendicular, &ptBottomCenter) != QLineF::NoIntersection) {
            lineHeight.setP2(ptBottomCenter);
            m_ptCenter = lineHeight.center();
            m_size.setHeight(lineHeight.length());
            m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomCenter)] =
                CPhysValPoint(*m_pDrawingScene, ptBottomCenter, m_unit);
        }
    }
    m_fRadius = radius(m_size);
    m_fPhi_rad = fabs(angle_rad(m_size));
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopCenter));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    invalidateSelectionPoints();
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
    CPhysValPoint physValPt(*m_pDrawingScene, i_physValPoint.toQPointF(m_unit), m_unit);
    CPhysValPoint physValPtOpposite = bottomRight();
    // Diagonale from TopLeft to BottomRight:
    // - TopLeft.x may be right of BottomRight.x (dx < 0) -> Width is negative
    // - TopLeft.y may be below BottomRight.y (dy < 0)  -> Height is negative
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    m_fRadius = lineDiagonale.length() / 2.0;
    if (m_physValAngle.getVal() == 0.0) {
        m_size.setWidth(lineDiagonale.dx());
        m_size.setHeight(lineDiagonale.dy());
        m_fPhi_rad = fabs(angle_rad(m_size));
    }
    else {
        double fBeta = fabs(angle_rad(QSizeF(lineDiagonale.dx(), lineDiagonale.dy())));
        m_fPhi_rad = fBeta - m_physValAngle.getVal(Units.Angle.Rad);
        m_size.setWidth(2.0 * m_fRadius * cos(m_fPhi_rad));
        m_size.setHeight(2.0 * m_fRadius * sin(m_fPhi_rad));
    }
    m_arphysValPoints[static_cast<int>(ESelectionPoint::TopLeft)] = physValPt;
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    CPhysValPoint physValPt(*m_pDrawingScene, i_physValPoint.toQPointF(m_unit), m_unit);
    CPhysValPoint physValPtOpposite = bottomLeft();
    // Diagonale from TopRight to BottomLeft:
    // - TopRight.x may be left of BottomLeft.x (dx > 0) -> Width is negative
    // - TopRight.y may be below BottomLeft.y (dy < 0) -> Height is negative
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    m_fRadius = lineDiagonale.length() / 2.0;
    if (m_physValAngle.getVal() == 0.0) {
        m_size.setWidth(-lineDiagonale.dx());
        m_size.setHeight(lineDiagonale.dy());
        m_fPhi_rad = fabs(angle_rad(m_size));
    }
    else {
        double fBeta = fabs(angle_rad(QSizeF(lineDiagonale.dx(), lineDiagonale.dy())));
        m_fPhi_rad = fBeta + m_physValAngle.getVal(Units.Angle.Rad);
        m_size.setWidth(2.0 * m_fRadius * cos(m_fPhi_rad));
        m_size.setHeight(2.0 * m_fRadius * sin(m_fPhi_rad));
    }
    m_arphysValPoints[static_cast<int>(ESelectionPoint::TopRight)] = physValPt;
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    CPhysValPoint physValPt(*m_pDrawingScene, i_physValPoint.toQPointF(m_unit), m_unit);
    CPhysValPoint physValPtOpposite = topLeft();
    // Diagonale from BottomRight to TopLeft:
    // - BottomRight.x may be left of TopLeft.x (dx > 0) -> Width is negative
    // - BottomRight.y may be above TopLeft.y (dy > 0) -> Height is negative
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    m_fRadius = lineDiagonale.length() / 2.0;
    if (m_physValAngle.getVal() == 0.0) {
        m_size.setWidth(-lineDiagonale.dx());
        m_size.setHeight(-lineDiagonale.dy());
        m_fPhi_rad = fabs(angle_rad(m_size));
    }
    else {
        double fBeta = fabs(angle_rad(QSizeF(lineDiagonale.dx(), lineDiagonale.dy())));
        m_fPhi_rad = fBeta - m_physValAngle.getVal(Units.Angle.Rad);
        m_size.setWidth(2.0 * m_fRadius * cos(m_fPhi_rad));
        m_size.setHeight(2.0 * m_fRadius * sin(m_fPhi_rad));
    }
    m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomRight)] = physValPt;
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
    CPhysValPoint physValPt(*m_pDrawingScene, i_physValPoint.toQPointF(m_unit), m_unit);
    CPhysValPoint physValPtOpposite = topRight();
    // Diagonale from BottomLeft to TopTight:
    // - BottomLeft.x may be right of TopTight.x (dx < 0) -> Width is negative
    // - BottomLeft.y may be above TopTight.y (dy > 0) -> Height is negative
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    m_fRadius = lineDiagonale.length() / 2.0;
    if (m_physValAngle.getVal() == 0.0) {
        m_size.setWidth(lineDiagonale.dx());
        m_size.setHeight(-lineDiagonale.dy());
        m_fPhi_rad = fabs(angle_rad(m_size));
    }
    else {
        double fBeta = fabs(angle_rad(QSizeF(lineDiagonale.dx(), lineDiagonale.dy())));
        m_fPhi_rad = fBeta + m_physValAngle.getVal(Units.Angle.Rad);
        m_size.setWidth(2.0 * m_fRadius * cos(m_fPhi_rad));
        m_size.setHeight(2.0 * m_fRadius * sin(m_fPhi_rad));
    }
    m_arphysValPoints[static_cast<int>(ESelectionPoint::BottomLeft)] = physValPt;
    quint16 uSelectionPointsToExclude = 0;
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
/*! @brief Outputs the rectangle as a string in format "TopLeft.x, TopLeft.y, Width, Height".

    @param [in] i_bAddUnit
        true to add the unit at the end of the string.
    @param [in] i_strSeparator (default ", ")
        Separator used to separate the values.
*/
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
/*! @brief Calculates the radius (half of the diagonal line) of a rectangle given
           by the size (width and height) of the rectangle.
*/
double CPhysValRect::radius(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    double fRadius = 0.0;
    // QSizeF::isValid cannot be used as the method returns false if either width
    // or height is less than 0.0. For metric units with YAxisScaleOrientation
    // BottomUp the top edge has lower values than the bottom edge and the height
    // is negative. Also negative widths are allowed.
    if (fabs(i_size.width()) > 0.0 || fabs(i_size.height()) > 0.0) {
        fRadius = Math::sqrt(Math::sqr(i_size.width()/2.0) + Math::sqr(i_size.height()/2.0));
    }
    return fRadius;
}

//------------------------------------------------------------------------------
/*! @brief Calculates the angle of the diagonal line of a rectangle given
           by the size (width and height) of the rectangle in radiant.

    If both width and height are positive the resulting angle is in range [0 .. PI/2.0] (0 .. 90°).

    @param [in] i_size
*/
double CPhysValRect::angle_rad(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    double fAngle_rad = (i_size.height() > 0.0) ? Math::c_f90Degrees_rad : Math::c_f270Degrees_rad;
    if (i_size.width() != 0.0) {
        fAngle_rad = atan(i_size.height()/i_size.width());
    }
    return fAngle_rad;
}

//------------------------------------------------------------------------------
/*! @brief Initializes the array with corner and other selection points and
           resets the calculated flags for each point.
*/
void CPhysValRect::initSelectionPoints()
//------------------------------------------------------------------------------
{
    m_arphysValPoints.clear();
    m_arbPointsCalculated.clear();
    // The first element is not used. This doesn't matter but simplifies the code.
    for (int selPt = 0; selPt <= ESelectionPointRectMax; ++selPt) {
        m_arphysValPoints.append(CPhysValPoint(*m_pDrawingScene));
        m_arbPointsCalculated.append(false);
    }
}

//------------------------------------------------------------------------------
/*! @brief Invalidates the calculated flag of the selection points excluding
           those for which the corresponding bit is set.

    @param [in] i_uSelectionPointsToExclude
        Bit set defining which selection points should not be reset.
        The bit number is defined by the enum ESelectionPoint.
        To exclude for example the top left corner call the method as follows:
            quint16 uSelectionPointsToExclude = 0;
            setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
            invalidateSelectionPoints(uSelectionPointsToExclude);
*/
void CPhysValRect::invalidateSelectionPoints(quint16 i_uSelectionPointsToExclude)
//------------------------------------------------------------------------------
{
    for (int selPt = ESelectionPointRectMin; selPt <= ESelectionPointRectMax; ++selPt) {
        if (!isBitSet(i_uSelectionPointsToExclude, static_cast<quint8>(selPt))) {
            m_arphysValPoints[selPt] = CPhysValPoint(*m_pDrawingScene);
            m_arbPointsCalculated[selPt] = false;
        }
    }
}

