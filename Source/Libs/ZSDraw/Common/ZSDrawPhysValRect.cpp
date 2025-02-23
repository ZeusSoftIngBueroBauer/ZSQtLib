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
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMath.h"

#include <QtGui/qpainter.h>

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValRect
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical rectangle on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_ptCenter(),
    m_size(0.0, 0.0),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical rectangle on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_ptCenter(),
    m_size(0.0, 0.0),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene, const QRectF& i_rect) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.width(), i_rect.height()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_rect.topLeft().y() > i_rect.bottomRight().y()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_rect.width() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_rect.height() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const QRectF& i_rect,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_ptCenter(i_rect.center()),
    m_size(i_rect.width(), i_rect.height()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_rect.topLeft().y() > i_rect.bottomRight().y()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_rect.width() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_rect.height() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (!m_bYAxisTopDown) {
        m_ptCenter.setY(i_rect.topLeft().y() - i_rect.size().height()/2.0);
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_ptTL, const QPointF& i_ptBR) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_ptCenter(QLineF(i_ptTL, i_ptBR).center()),
    m_size(i_ptBR.x() - i_ptTL.x(), fabs(i_ptBR.y() - i_ptTL.y())),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_ptTL.x() > i_ptBR.x()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (m_bYAxisTopDown) {
        if (i_ptTL.y() > i_ptBR.y()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    else {
        if (i_ptTL.y() < i_ptBR.y()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_ptTL, const QPointF& i_ptBR,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_ptCenter(QLineF(i_ptTL, i_ptBR).center()),
    m_size(i_ptBR.x() - i_ptTL.x(), fabs(i_ptBR.y() - i_ptTL.y())),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_ptTL.x() > i_ptBR.x()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (m_bYAxisTopDown) {
        if (i_ptTL.y() > i_ptBR.y()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    else {
        if (i_ptTL.y() < i_ptBR.y()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_ptTL, const QSizeF& i_size) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_ptCenter(),
    m_size(i_size),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_size.width() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_size.height() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    m_ptCenter.setX(i_ptTL.x() + i_size.width()/2.0);
    if (m_bYAxisTopDown) {
        m_ptCenter.setY(i_ptTL.y() + i_size.height()/2.0);
    }
    else {
        m_ptCenter.setY(i_ptTL.y() - i_size.height()/2.0);
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_ptTL, const QSizeF& i_size,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_ptCenter(),
    m_size(i_size),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (i_size.width() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_size.height() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    m_ptCenter.setX(i_ptTL.x() + i_size.width()/2.0);
    if (m_bYAxisTopDown) {
        m_ptCenter.setY(i_ptTL.y() + i_size.height()/2.0);
    }
    else {
        m_ptCenter.setY(i_ptTL.y() - i_size.height()/2.0);
    }
    initSelectionPoints();
}

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValPoint& i_physValBottomRight) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValTopLeft.drawingScene(), i_physValTopLeft.unit()),
    m_ptCenter(QLineF(i_physValTopLeft.toQPointF(), i_physValBottomRight.toQPointF()).center()),
    m_size(fabs(i_physValTopLeft.x().getVal() - i_physValBottomRight.x().getVal()),
           fabs(i_physValTopLeft.y().getVal() - i_physValBottomRight.y().getVal())),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (&i_physValTopLeft.drawingScene() != &i_physValBottomRight.drawingScene()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.unit() != i_physValBottomRight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValBottomRight.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (m_bYAxisTopDown) {
        if (i_physValTopLeft.y().getVal() > i_physValBottomRight.y().getVal()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    else {
        if (i_physValTopLeft.y().getVal() < i_physValBottomRight.y().getVal()) {
            throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
        }
    }
    initSelectionPoints();
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValSize& i_physValSize) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValTopLeft.drawingScene(), i_physValTopLeft.unit()),
    m_ptCenter(),
    m_size(i_physValSize.toQSizeF()),
    m_physValAngle(0.0, Units.Angle.Degree, 0.1),
    m_arphysValRectSelectionPoints(),
    m_arbRectSelectionPointsCalculated()
{
    if (&i_physValTopLeft.drawingScene() != &i_physValSize.drawingScene()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.unit() != i_physValSize.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValSize.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValSize.width().getVal() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    else if (i_physValSize.height().getVal() < 0.0) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    m_ptCenter.setX(i_physValTopLeft.x().getVal() + i_physValSize.width().getVal()/2.0);
    if (m_bYAxisTopDown) {
        m_ptCenter.setY(i_physValTopLeft.y().getVal() + i_physValSize.height().getVal()/2.0);
    }
    else {
        m_ptCenter.setY(i_physValTopLeft.y().getVal() - i_physValSize.height().getVal()/2.0);
    }
    initSelectionPoints();
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValRectOther),
    m_ptCenter(i_physValRectOther.m_ptCenter),
    m_size(i_physValRectOther.m_size),
    m_physValAngle(i_physValRectOther.m_physValAngle),
    m_arphysValRectSelectionPoints(i_physValRectOther.m_arphysValRectSelectionPoints),
    m_arbRectSelectionPointsCalculated(i_physValRectOther.m_arbRectSelectionPointsCalculated)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    //m_ptCenter;
    //m_size;
    //m_physValAngle;
    //m_arphysValRectSelectionPoints;
    //m_arbRectSelectionPointsCalculated;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const QRectF& i_rectOther )
//------------------------------------------------------------------------------
{
    m_ptCenter = i_rectOther.center();
    m_size = i_rectOther.size();
    initSelectionPoints();
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValRect::operator = ( const CPhysValShape& i_physValRectOther )
//------------------------------------------------------------------------------
{
    const CPhysValRect& physValRectOther = dynamic_cast<const CPhysValRect&>(i_physValRectOther);
    m_bYAxisTopDown = physValRectOther.m_bYAxisTopDown;
    m_unit = physValRectOther.m_unit;
    m_ptCenter = physValRectOther.m_ptCenter;
    m_size = physValRectOther.m_size;
    m_physValAngle = physValRectOther.m_physValAngle;
    m_arphysValRectSelectionPoints = physValRectOther.m_arphysValRectSelectionPoints;
    m_arbRectSelectionPointsCalculated = physValRectOther.m_arbRectSelectionPointsCalculated;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValRect::operator == ( const CPhysValShape& i_physValRectOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    const CPhysValRect& physValRectOther = dynamic_cast<const CPhysValRect&>(i_physValRectOther);
    if (!areOfSameUnitGroup(m_unit, physValRectOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit != physValRectOther.m_unit) {
        bEqual = false;
    }
    else if (m_ptCenter != physValRectOther.m_ptCenter) {
        bEqual = false;
    }
    else if (m_size != physValRectOther.m_size) {
        bEqual = false;
    }
    else if (m_physValAngle != physValRectOther.m_physValAngle) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValRect::operator != ( const CPhysValShape& i_physValRectOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValRectOther);
}

/*==============================================================================
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the coordinates by setting width and height to 0.0 and by
           invalidating the calculated selection points.
*/
void CPhysValRect::invalidate()
//------------------------------------------------------------------------------
{
    m_ptCenter = QPointF();
    m_size = QSizeF();
    m_physValAngle = CPhysVal(0.0, Units.Angle.Degree, 0.1);
    invalidateSelectionPoints();
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is valid, otherwise returns false.

    A valid rectangle has both width and height greater or equal than 0.0.
*/
bool CPhysValRect::isValid() const
//------------------------------------------------------------------------------
{
    return m_size.isValid();
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is null, otherwise returns false.

    A null rectangle has both width and height equal to 0.0.
*/
bool CPhysValRect::isNull() const
//------------------------------------------------------------------------------
{
    return m_size.isNull();
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValRect::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
    i_pPainter->save();
    QPen pen(i_drawSettings.getPenColor());
    QPointF ptCenter = m_pDrawingScene->convert(center(), Units.Length.px).toQPointF();
    QPointF ptTL = m_pDrawingScene->convert(topLeft(), Units.Length.px).toQPointF();
    QPointF ptTR = m_pDrawingScene->convert(topRight(), Units.Length.px).toQPointF();
    QPointF ptBR = m_pDrawingScene->convert(bottomRight(), Units.Length.px).toQPointF();
    QPointF ptBL = m_pDrawingScene->convert(bottomLeft(), Units.Length.px).toQPointF();
    QPointF ptTC = m_pDrawingScene->convert(topCenter(), Units.Length.px).toQPointF();
    QPointF ptRC = m_pDrawingScene->convert(rightCenter(), Units.Length.px).toQPointF();
    QPointF ptBC = m_pDrawingScene->convert(bottomCenter(), Units.Length.px).toQPointF();
    QPointF ptLC = m_pDrawingScene->convert(leftCenter(), Units.Length.px).toQPointF();
    pen.setStyle(Qt::SolidLine);
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
        i_pPainter->drawText(ptCenter, "X");
        i_pPainter->drawText(ptTL, "TL");
        i_pPainter->drawText(ptTR, "TR");
        i_pPainter->drawText(ptBR, "BR");
        i_pPainter->drawText(ptBL, "BL");
        i_pPainter->drawText(ptTC, "TC");
        i_pPainter->drawText(ptRC, "RC");
        i_pPainter->drawText(ptBC, "BC");
        i_pPainter->drawText(ptLC, "LC");
    }
    i_pPainter->restore();
}

//------------------------------------------------------------------------------
/*! @brief Writes the point to coordinates into a string in the format
           "Center.x, Center.y, Width, Height[ unit.symbol], angle unit.symbol".

    @param [in] i_bAddUnit
        true to append the unit at the end of the coordinates.
    @param [in] i_strSeparator
        String to separate the coordinates
    @param [in] i_iPrecision
        If >= 0 overwrites the precision (number of digits after the decimal point)
        as defined by the internal resolution of the point.
*/
QString CPhysValRect::toString(bool i_bAddUnit, const QString& i_strSeparator, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QString str;
    if (!isValid()) {
        str = "Invalid";
    }
    else if (isNull()) {
        str = "Null";
    }
    else if (i_iPrecision < 0) {
        str = center().x().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + center().y().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + size().width().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + size().height().toString(EUnitFind::None, PhysValSubStr::Val);
    }
    else {
        str = QString::number(Math::round2Nearest(center().x().getVal(), i_iPrecision), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(Math::round2Nearest(center().y().getVal(), i_iPrecision), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(Math::round2Nearest(size().width().getVal(), i_iPrecision), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(Math::round2Nearest(size().height().getVal(), i_iPrecision), 'f', i_iPrecision);
    }
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    str += ", " + m_physValAngle.toString();
    return str;
}

/*==============================================================================
public: // overridables
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
*/
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopLeft)]) {
        QPointF pt(m_ptCenter.x() - m_size.width() / 2.0, m_ptCenter.y() - m_size.height() / 2.0);
        double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
        if (fAngle_degree != 0.0) {
            double fAngle1_degree = Math::toCounterClockWiseAngleDegree(fAngle_degree);
            double fAngle_rad = Math::degree2Rad(fAngle1_degree);
            // rotatePoint assumes a y scale from Top to Bottom.
            pt = ZS::Draw::rotatePoint(m_ptCenter, pt, fAngle_rad);
        }
        if (!m_bYAxisTopDown) {
            double dy = m_ptCenter.y() - pt.y();
            pt.setY(m_ptCenter.y() + dy);
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopLeft)] = CPhysValPoint(*m_pDrawingScene, pt, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopLeft)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopLeft)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the top right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopRight)]) {
        QPointF pt(m_ptCenter.x() + m_size.width() / 2.0, m_ptCenter.y() - m_size.height() / 2.0);
        double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
        if (fAngle_degree != 0.0) {
            double fAngle1_degree = Math::toCounterClockWiseAngleDegree(fAngle_degree);
            double fAngle_rad = Math::degree2Rad(fAngle1_degree);
            // rotatePoint assumes a y scale from Top to Bottom.
            pt = ZS::Draw::rotatePoint(m_ptCenter, pt, fAngle_rad);
        }
        if (!m_bYAxisTopDown) {
            double dy = m_ptCenter.y() - pt.y();
            pt.setY(m_ptCenter.y() + dy);
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopRight)] = CPhysValPoint(*m_pDrawingScene, pt, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopRight)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopRight)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom right corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomRight)]) {
        QPointF pt(m_ptCenter.x() + m_size.width() / 2.0, m_ptCenter.y() + m_size.height() / 2.0);
        double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
        if (fAngle_degree != 0.0) {
            double fAngle1_degree = Math::toCounterClockWiseAngleDegree(fAngle_degree);
            double fAngle_rad = Math::degree2Rad(fAngle1_degree);
            // rotatePoint assumes a y scale from Top to Bottom.
            pt = ZS::Draw::rotatePoint(m_ptCenter, pt, fAngle_rad);
        }
        if (!m_bYAxisTopDown) {
            double dy = m_ptCenter.y() - pt.y();
            pt.setY(m_ptCenter.y() + dy);
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomRight)] = CPhysValPoint(*m_pDrawingScene, pt, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomRight)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomRight)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom left corner of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomLeft)]) {
        QPointF pt(m_ptCenter.x() - m_size.width() / 2.0, m_ptCenter.y() + m_size.height() / 2.0);
        double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
        if (fAngle_degree != 0.0) {
            double fAngle1_degree = Math::toCounterClockWiseAngleDegree(fAngle_degree);
            double fAngle_rad = Math::degree2Rad(fAngle1_degree);
            // rotatePoint assumes a y scale from Top to Bottom.
            pt = ZS::Draw::rotatePoint(m_ptCenter, pt, fAngle_rad);
        }
        if (!m_bYAxisTopDown) {
            double dy = m_ptCenter.y() - pt.y();
            pt.setY(m_ptCenter.y() + dy);
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomLeft)] = CPhysValPoint(*m_pDrawingScene, pt, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomLeft)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomLeft)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the top center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::topCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopCenter)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        if (m_physValAngle.getVal() == 0.0) {
            if (m_bYAxisTopDown) {
                fY -= m_size.height() / 2.0;
            }
            else {
                fY += m_size.height() / 2.0;
            }
        }
        else {
            CPhysValPoint physValPtTL = topLeft();
            CPhysValPoint physValPtTR = topRight();
            fX = (physValPtTL.x().getVal() + physValPtTR.x().getVal()) / 2.0;
            fY = (physValPtTL.y().getVal() + physValPtTR.y().getVal()) / 2.0;
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopCenter)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the right center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::rightCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::RightCenter)]) {
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
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::RightCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::RightCenter)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::RightCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the bottom center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::bottomCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomCenter)]) {
        double fX = m_ptCenter.x();
        double fY = m_ptCenter.y();
        if (m_physValAngle.getVal() == 0.0) {
            if (m_bYAxisTopDown) {
                fY += m_size.height() / 2.0;
            }
            else {
                fY -= m_size.height() / 2.0;
            }
        }
        else {
            CPhysValPoint physValPtBR = bottomRight();
            CPhysValPoint physValPtBL = bottomLeft();
            fX = (physValPtBR.x().getVal() + physValPtBL.x().getVal()) / 2.0;
            fY = (physValPtBR.y().getVal() + physValPtBL.y().getVal()) / 2.0;
        }
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomCenter)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the left center point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::leftCenter() const
//------------------------------------------------------------------------------
{
    if (!m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::LeftCenter)]) {
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
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::LeftCenter)] = CPhysValPoint(*m_pDrawingScene, fX, fY, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::LeftCenter)] = true;
    }
    return m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::LeftCenter)];
}

//------------------------------------------------------------------------------
/*! @brief Returns the given selection point of the rectangle.

    As the rectangle may be rotated the point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
CPhysValPoint CPhysValRect::selectionPoint(ESelectionPoint i_selPt) const
//------------------------------------------------------------------------------
{
    CPhysValPoint physValPoint(center());
    if (i_selPt == ESelectionPoint::TopLeft) {
        physValPoint = topLeft();
    }
    else if (i_selPt == ESelectionPoint::TopRight) {
        physValPoint = topRight();
    }
    else if (i_selPt == ESelectionPoint::BottomRight) {
        physValPoint = bottomRight();
    }
    else if (i_selPt == ESelectionPoint::BottomLeft) {
        physValPoint = bottomLeft();
    }
    else if (i_selPt == ESelectionPoint::TopCenter) {
        physValPoint = topCenter();
    }
    else if (i_selPt == ESelectionPoint::RightCenter) {
        physValPoint = rightCenter();
    }
    else if (i_selPt == ESelectionPoint::BottomCenter) {
        physValPoint = bottomCenter();
    }
    else if (i_selPt == ESelectionPoint::LeftCenter) {
        physValPoint = leftCenter();
    }
    else if (i_selPt == ESelectionPoint::Center) {
        physValPoint = center();
    }
    return physValPoint;
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
void CPhysValRect::setCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    setCenter(CPhysValPoint(*m_pDrawingScene, i_pt, m_unit));
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

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

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

    On changing the size the rotation angle of the rectangle remains the same.

    If the Y axis scale orientation is from top to bottom also the top left
    corner remains unchanged. For scale orientation from bottom to top the
    bottom left corner remains unchanged.

    To keep either the top left or bottom left corner the center point will be moved.

    Width and height must be greater than or equal to 0. For an invalid size an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValSize.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QSizeF sizeF = i_physValSize.toQSizeF(m_unit);
    //if (!sizeF.isValid()) {
    //    throw CException(__FILE__, __LINE__, EResultInvalidSize);
    //}
    setWidth(i_physValSize.width());
    setHeight(i_physValSize.height());
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

    Width must be greater or equal than 0. For an invalid value an exception is thrown.

    As the rectangle may be rotated the new center point must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValWidth.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    double fWidth = i_physValWidth.getVal(m_unit);
    if (fWidth < 0.0) {
        throw CException(__FILE__, __LINE__, EResultInvalidValue);
    }
    if (fWidth > 0.0) {
        QLineF lineWidth(leftCenter().toQPointF(), rightCenter().toQPointF());
        lineWidth.setLength(fWidth);
        m_ptCenter = lineWidth.center();
    }
    else {
        QLineF lineHeight(topCenter().toQPointF(), bottomCenter().toQPointF());
        m_ptCenter = lineHeight.center();
    }
    m_size.setWidth(fWidth);
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
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
void CPhysValRect::setWidthByMovingLeftCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::RightCenter));
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
    if (fAngle_degree == 0.0 || fAngle_degree == 180.0) {
        QPointF ptRightCenter = rightCenter().toQPointF();
        QPointF ptLeftCenter = leftCenter().toQPointF();
        ptLeftCenter.setX(ptMoved.x());
        QLineF lineWidth(ptLeftCenter, ptRightCenter);
        m_ptCenter = lineWidth.center();
        m_size.setWidth(fabs(lineWidth.dx()));
        // Width and height must never be less than 0.
        if (fAngle_degree == 0.0 && lineWidth.dx() < 0.0) {
            m_physValAngle.setVal(180.0);
        }
        else if (fAngle_degree == 180.0 && lineWidth.dx() > 0.0) {
            m_physValAngle.setVal(0.0);
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
        }
    }
    else {
        // Create the widthLine from LC' to RC' selection points.
        QPointF ptLeftCenter = leftCenter().toQPointF();
        QPointF ptRightCenter = rightCenter().toQPointF();
        QLineF lineWidth(ptLeftCenter, ptRightCenter);
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle1_degree = lineWidth.angle();
        // Correct angle only if not already clockwise counted:
        fAngle1_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle1_degree) : fAngle1_degree;
        // Determine the perpendicularLine to the widthLine going through ptPosMoved
        // and the intersection point LC'' of the perpendicular to the width line.
        QLineF linePerpendicular = getPerpendicularLine(lineWidth, ptMoved, 0.0, &ptLeftCenter);
        // Get length of line from LC'' to RC' and use this as the new width w'' of the rectangle
        lineWidth.setP1(ptLeftCenter);
        double fWidth = lineWidth.length();
        // Get rotation angle α'' of the new widthLine'' and use this as the new rotation angle of the rectangle.
        // If LC has not been moved beyond the right center selection point, the rotation angle α remains the same.
        // Otherwise 180° has to be added to the rotation angle.
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle2_degree = lineWidth.angle();
        // Correct angle only if not already clockwise counted:
        fAngle2_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle2_degree) : fAngle2_degree;
        double fAngleHorLine_degree = Math::normalizeAngleInDegree(fAngle2_degree - m_physValAngle.getVal(Units.Angle.Degree));
        if (fAngleHorLine_degree > 179.0 && fAngleHorLine_degree < 181.0) {
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
        }
        // Get center point of line from LC'' to RC' and use this as the new center point of the rectangle.
        m_ptCenter = lineWidth.center();
        m_size.setWidth(fWidth);
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::LeftCenter)] =
            CPhysValPoint(*m_pDrawingScene, ptLeftCenter, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::LeftCenter)] = true;
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    }
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

    TODO

    As the rectangle may be rotated the new size must be calculated using trigonometric
    functions applied to the distance (radius) of the corner point to the center point.
*/
void CPhysValRect::setWidthByMovingRightCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValPoint.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::LeftCenter));
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
    if (fAngle_degree == 0.0 || fAngle_degree == 180.0) {
        QPointF ptLeftCenter = leftCenter().toQPointF();
        QPointF ptRightCenter = rightCenter().toQPointF();
        ptRightCenter.setX(ptMoved.x());
        QLineF lineWidth(ptLeftCenter, ptRightCenter);
        m_ptCenter = lineWidth.center();
        m_size.setWidth(fabs(lineWidth.dx()));
        // Width and height must never be less than 0.
        if (fAngle_degree == 0.0 && lineWidth.dx() < 0.0) {
            m_physValAngle.setVal(180.0);
        }
        else if (fAngle_degree == 180.0 && lineWidth.dx() > 0.0) {
            m_physValAngle.setVal(0.0);
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
        }
    }
    else {
        // Create the widthLine from LC' to RC' selection points.
        QPointF ptLeftCenter = leftCenter().toQPointF();
        QPointF ptRightCenter = rightCenter().toQPointF();
        QLineF lineWidth(ptLeftCenter, ptRightCenter);
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle1_degree = lineWidth.angle();
        // Correct angle only if not already clockwise counted:
        fAngle1_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle1_degree) : fAngle1_degree;
        // Determine the perpendicularLine to the widthLine going through ptPosMoved
        // and the intersection point RC'' of the perpendicular to the width line.
        QLineF linePerpendicular = getPerpendicularLine(lineWidth, ptMoved, 0.0, &ptRightCenter);
        // Get length of line from RC'' to LC' and use this as the new width w'' of the rectangle
        lineWidth.setP2(ptRightCenter);
        double fWidth = lineWidth.length();
        // Get rotation angle α'' of the new widthLine'' and use this as the new rotation angle of the rectangle.
        // If RC has not been moved beyond the left center selection point, the rotation angle α remains the same.
        // Otherwise 180° has to be added to the rotation angle.
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle2_degree = lineWidth.angle();
        // Correct angle only if not already clockwise counted:
        fAngle2_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle2_degree) : fAngle2_degree;
        double fAngleHorLine_degree = Math::normalizeAngleInDegree(fAngle2_degree - m_physValAngle.getVal(Units.Angle.Degree));
        if (fAngleHorLine_degree > 179.0 && fAngleHorLine_degree < 181.0) {
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
        }
        // Get center point of line from LC'' to RC' and use this as the new center point of the rectangle.
        m_ptCenter = lineWidth.center();
        m_size.setWidth(fWidth);
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::RightCenter)] =
            CPhysValPoint(*m_pDrawingScene, ptRightCenter, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::RightCenter)] = true;
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::RightCenter));
    }
    invalidateSelectionPoints(uSelectionPointsToExclude);
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
void CPhysValRect::setHeight(double i_fHeight)
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
void CPhysValRect::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(m_unit, i_physValHeight.unit())) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    double fHeight = i_physValHeight.getVal(m_unit);
    if (fHeight < 0.0) {
        throw CException(__FILE__, __LINE__, EResultInvalidValue);
    }
    if (fHeight > 0.0) {
        QPointF ptStart(m_bYAxisTopDown ? topCenter().toQPointF() : bottomCenter().toQPointF());
        QPointF ptEnd(m_bYAxisTopDown ? bottomCenter().toQPointF() : topCenter().toQPointF());
        QLineF lineHeight(ptStart, ptEnd);
        lineHeight.setLength(fHeight);
        m_ptCenter = lineHeight.center();
    }
    else {
        QLineF lineWidth(leftCenter().toQPointF(), rightCenter().toQPointF());
        m_ptCenter = lineWidth.center();
    }
    m_size.setHeight(fHeight);
    quint16 uSelectionPointsToExclude = 0;
    if (m_bYAxisTopDown) {
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopCenter));
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    }
    else {
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomCenter));
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    }
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
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomCenter));
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
    if (fAngle_degree == 0.0 || fAngle_degree == 180.0) {
        QPointF ptBottomCenter = bottomCenter().toQPointF();
        QPointF ptTopCenter = topCenter().toQPointF();
        ptTopCenter.setY(ptMoved.y());
        QLineF lineHeight(ptTopCenter, ptBottomCenter);
        m_ptCenter = lineHeight.center();
        m_size.setHeight(fabs(lineHeight.dy()));
        // Width and height must never be less than 0.
        if (m_bYAxisTopDown) {
            if (fAngle_degree == 0.0 && lineHeight.dy() < 0.0) {
                m_physValAngle.setVal(180.0);
            }
            else if (fAngle_degree == 180.0 && lineHeight.dy() > 0.0) {
                m_physValAngle.setVal(0.0);
            }
            else {
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
            }
        }
        else {
            if (fAngle_degree == 0.0 && lineHeight.dy() > 0.0) {
                m_physValAngle.setVal(180.0);
            }
            else if (fAngle_degree == 180.0 && lineHeight.dy() < 0.0) {
                m_physValAngle.setVal(0.0);
            }
            else {
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
            }
        }
    }
    else {
        // Create the heightLine from TC' to BC' selection points.
        QPointF ptTopCenter = topCenter().toQPointF();
        QPointF ptBottomCenter = bottomCenter().toQPointF();
        QLineF lineHeight(ptTopCenter, ptBottomCenter);
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle1_degree = lineHeight.angle();
        // Correct angle only if not already clockwise counted:
        fAngle1_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle1_degree) : fAngle1_degree;
        // Determine the perpendicularLine to the heightLine going through ptPosMoved
        // and the intersection point TC'' of the perpendicular to the height line.
        QLineF linePerpendicular = getPerpendicularLine(lineHeight, ptMoved, 0.0, &ptTopCenter);
        lineHeight.setP1(ptTopCenter);
        double fHeight = lineHeight.length();
        // Get rotation angle α'' of the new heightLine'', add 90° and use this as the new rotation angle of the rectangle.
        // If TC has not been moved beyond the bottom center selection point, the rotation angle α remains the same.
        // Otherwise 180° has to be added to the rotation angle.
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle2_degree = lineHeight.angle();
        // Correct angle only if not already clockwise counted:
        fAngle2_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle2_degree) : fAngle2_degree;
        double fAngleVerLine_degree = Math::normalizeAngleInDegree(fAngle2_degree - m_physValAngle.getVal(Units.Angle.Degree));
        if (fAngleVerLine_degree > 269.0 && fAngleVerLine_degree < 271.0) {
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
        }
        // Get center point of line from TC'' to BC' and use this as the new center point of the rectangle.
        m_ptCenter = lineHeight.center();
        m_size.setHeight(fHeight);
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopCenter)] =
            CPhysValPoint(*m_pDrawingScene, ptTopCenter, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopCenter)] = true;
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopCenter));
    }
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
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopCenter));
    QPointF ptMoved = i_physValPoint.toQPointF(m_unit);
    double fAngle_degree = m_physValAngle.getVal(Units.Angle.Degree);
    if (fAngle_degree == 0.0 || fAngle_degree == 180.0) {
        QPointF ptTopCenter = topCenter().toQPointF();
        QPointF ptBottomCenter = bottomCenter().toQPointF();
        ptBottomCenter.setY(ptMoved.y());
        QLineF lineHeight(ptTopCenter, ptBottomCenter);
        m_ptCenter = lineHeight.center();
        m_size.setHeight(fabs(lineHeight.dy()));
        // Width and height must never be less than 0.
        if (m_bYAxisTopDown) {
            if (fAngle_degree == 0.0 && lineHeight.dy() < 0.0) {
                m_physValAngle.setVal(180.0);
            }
            else if (fAngle_degree == 180.0 && lineHeight.dy() > 0.0) {
                m_physValAngle.setVal(0.0);
            }
            else {
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
            }
        }
        else {
            if (fAngle_degree == 0.0 && lineHeight.dy() > 0.0) {
                m_physValAngle.setVal(180.0);
            }
            else if (fAngle_degree == 180.0 && lineHeight.dy() < 0.0) {
                m_physValAngle.setVal(0.0);
            }
            else {
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
                ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
            }
        }
    }
    else {
        // Create the heightLine from TC' to BC' selection points.
        QPointF ptTopCenter = topCenter().toQPointF();
        QPointF ptBottomCenter = bottomCenter().toQPointF();
        QLineF lineHeight(ptTopCenter, ptBottomCenter);
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle1_degree = lineHeight.angle();
        // Correct angle only if not already clockwise counted:
        fAngle1_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle1_degree) : fAngle1_degree;
        // Determine the perpendicularLine to the heightLine going through ptPosMoved
        // and the intersection point BC'' of the perpendicular to the height line.
        QLineF linePerpendicular = getPerpendicularLine(lineHeight, ptMoved, 0.0, &ptBottomCenter);
        lineHeight.setP2(ptBottomCenter);
        double fHeight = lineHeight.length();
        // Get rotation angle α'' of the new heightLine'', add 90° and use this as the new rotation angle of the rectangle.
        // If BC has not been moved beyond the top center selection point, the rotation angle α remains the same.
        // Otherwise 180° has to be added to the rotation angle.
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fAngle2_degree = lineHeight.angle();
        // Correct angle only if not already clockwise counted:
        fAngle2_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fAngle2_degree) : fAngle2_degree;
        double fAngleVerLine_degree = Math::normalizeAngleInDegree(fAngle2_degree - m_physValAngle.getVal(Units.Angle.Degree));
        if (fAngleVerLine_degree > 269.0 && fAngleVerLine_degree < 271.0) {
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else {
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
            ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
        }
        // Get center point of line from TC'' to BC' and use this as the new center point of the rectangle.
        m_ptCenter = lineHeight.center();
        m_size.setHeight(fHeight);
        m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomCenter)] =
            CPhysValPoint(*m_pDrawingScene, ptBottomCenter, m_unit);
        m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomCenter)] = true;
        ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomCenter));
    }
    invalidateSelectionPoints(uSelectionPointsToExclude);
}

//------------------------------------------------------------------------------
/*! @brief Sets the rotation angle of the rectangle in degrees.

    The center point, width and height of the rectangle remain unchanged.
    The rectangle's edges are implicitly changed.

    The angles are measured clockwise with 0° at 3 o'clock (90° at 6 o'clock).

    @param [in] i_physValAngle
        Angle to be set.
*/
void CPhysValRect::setAngle( double i_fAngle_degree )
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
void CPhysValRect::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    CPhysVal physValAngle = i_physValAngle;
    double fAngle = i_physValAngle.getVal();
    if (i_physValAngle.unit() == Units.Angle.Degree) {
        fAngle = Math::normalizeAngleInDegree(fAngle);
    }
    else {
        fAngle = Math::normalizeAngleInRad(fAngle);
    }
    physValAngle.setVal(fAngle);
    m_physValAngle = physValAngle;
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
    // Diagonale from TopLeft to BottomRight.
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    if (m_physValAngle.getVal() == 0.0 || m_physValAngle.getVal() == 90.0
     || m_physValAngle.getVal() == 180.0 || m_physValAngle.getVal() == 270.0) {
        if (m_bYAxisTopDown) {
            if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
        else {
            if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
    }
    else {
        double fDiameter = lineDiagonale.length();
        // QLineF::angle returns the angle counter clockwise (3 o'cock = 0°, 12 o'clock = 90°).
        // The grapics libaries angles are measure clockwise (3 o'clock = 0°, 6 o'clock = 90°, 12 o'clock = 270°).
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fBeta_degree = lineDiagonale.angle();
        // Correct angle only if not already clockwise counted:
        fBeta_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fBeta_degree) : fBeta_degree;
        double fPhi_degree = Math::normalizeAngleInDegree(fBeta_degree - m_physValAngle.getVal(Units.Angle.Degree));
        // The trigonometric functions of the stdlib are using counter clockwise angles in radiant.
        double fPhi_rad = Math::degree2Rad(Math::toCounterClockWiseAngleDegree(fPhi_degree));
        double fWidth = fabs(fDiameter * cos(fPhi_rad));
        double fHeight = fabs(fDiameter * sin(fPhi_rad));
        if (fPhi_degree >= 0.0 && fPhi_degree < 90.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
        }
        else if (fPhi_degree >= 90.0 && fPhi_degree < 180.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 90.0));
        }
        else if (fPhi_degree >= 180.0 && fPhi_degree < 270.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else /*if (fPhi_degree >= 270.0 && fPhi_degree < 360.0)*/ {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 270.0));
        }
    }
    m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopLeft)] = physValPt;
    m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopLeft)] = true;
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
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
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    if (m_physValAngle.getVal() == 0.0 || m_physValAngle.getVal() == 90.0
     || m_physValAngle.getVal() == 180.0 || m_physValAngle.getVal() == 270.0) {
        if (m_bYAxisTopDown) {
            if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
        else {
            if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
    }
    else {
        double fDiameter = lineDiagonale.length();
        // QLineF::angle returns the angle counter clockwise (3 o'cock = 0°, 12 o'clock = 90°).
        // The grapics libaries angles are measure clockwise (3 o'clock = 0°, 6 o'clock = 90°, 12 o'clock = 270°).
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fBeta_degree = lineDiagonale.angle();
        // Correct angle only if not already clockwise counted:
        fBeta_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fBeta_degree) : fBeta_degree;
        double fPhi_degree = Math::normalizeAngleInDegree(fBeta_degree - m_physValAngle.getVal(Units.Angle.Degree));
        // The trigonometric functions of the stdlib are using counter clockwise angles in radiant.
        double fPhi_rad = Math::degree2Rad(Math::toCounterClockWiseAngleDegree(fPhi_degree));
        double fWidth = fabs(fDiameter * cos(fPhi_rad));
        double fHeight = fabs(fDiameter * sin(fPhi_rad));
        if (fPhi_degree >= 0.0 && fPhi_degree < 90.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 270.0));
        }
        else if (fPhi_degree >= 90.0 && fPhi_degree < 180.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
        }
        else if (fPhi_degree >= 180.0 && fPhi_degree < 270.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 90.0));
        }
        else /*if (fPhi_degree >= 270.0 && fPhi_degree < 360.0)*/ {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
    }
    m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::TopRight)] = physValPt;
    m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::TopRight)] = true;
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
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
    QLineF lineDiagonale(physValPt.toQPointF(), physValPtOpposite.toQPointF());
    m_ptCenter = lineDiagonale.center();
    if (m_physValAngle.getVal() == 0.0 || m_physValAngle.getVal() == 90.0
     || m_physValAngle.getVal() == 180.0 || m_physValAngle.getVal() == 270.0) {
        if (m_bYAxisTopDown) {
            if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
        else {
            if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
    }
    else {
        double fDiameter = lineDiagonale.length();
        // QLineF::angle returns the angle counter clockwise (3 o'cock = 0°, 12 o'clock = 90°).
        // The grapics libaries angles are measure clockwise (3 o'clock = 0°, 6 o'clock = 90°, 12 o'clock = 270°).
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fBeta_degree = lineDiagonale.angle();
        // Correct angle only if not already clockwise counted:
        fBeta_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fBeta_degree) : fBeta_degree;
        double fPhi_degree = Math::normalizeAngleInDegree(fBeta_degree - m_physValAngle.getVal(Units.Angle.Degree));
        // The trigonometric functions of the stdlib are using counter clockwise angles in radiant.
        double fPhi_rad = Math::degree2Rad(Math::toCounterClockWiseAngleDegree(fPhi_degree));
        double fWidth = fabs(fDiameter * cos(fPhi_rad));
        double fHeight = fabs(fDiameter * sin(fPhi_rad));
        if (fPhi_degree >= 0.0 && fPhi_degree < 90.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else if (fPhi_degree >= 90.0 && fPhi_degree < 180.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 270.0));
        }
        else if (fPhi_degree >= 180.0 && fPhi_degree < 270.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
        }
        else /*if (fPhi_degree >= 270.0 && fPhi_degree < 360.0)*/ {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 90.0));
        }
    }
    m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomRight)] = physValPt;
    m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomRight)] = true;
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomRight));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopLeft));
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
    if (m_physValAngle.getVal() == 0.0 || m_physValAngle.getVal() == 90.0
     || m_physValAngle.getVal() == 180.0 || m_physValAngle.getVal() == 270.0) {
        if (m_bYAxisTopDown) {
            if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
        else {
            if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() >= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(0.0);
            }
            else if (lineDiagonale.dx() >= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(90.0);
            }
            else if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() <= 0.0) {
                m_size.setWidth(fabs(lineDiagonale.dx()));
                m_size.setHeight(fabs(lineDiagonale.dy()));
                m_physValAngle.setVal(180.0);
            }
            else /*if (lineDiagonale.dx() <= 0.0 && lineDiagonale.dy() >= 0.0)*/ {
                m_size.setWidth(fabs(lineDiagonale.dy()));
                m_size.setHeight(fabs(lineDiagonale.dx()));
                m_physValAngle.setVal(270.0);
            }
        }
    }
    else {
        double fDiameter = lineDiagonale.length();
        // QLineF::angle returns the angle counter clockwise (3 o'cock = 0°, 12 o'clock = 90°).
        // The grapics libaries angles are measure clockwise (3 o'clock = 0°, 6 o'clock = 90°, 12 o'clock = 270°).
        // For TopBottom Y-Scales the angle is returned counterclockwise counted.
        // For BottomUp Y-Scales the angle is returned clockwise counted.
        double fBeta_degree = lineDiagonale.angle();
        // Correct angle only if not already clockwise counted:
        fBeta_degree = m_bYAxisTopDown ? Math::toClockWiseAngleDegree(fBeta_degree) : fBeta_degree;
        double fPhi_degree = Math::normalizeAngleInDegree(fBeta_degree - m_physValAngle.getVal(Units.Angle.Degree));
        // The trigonometric functions of the stdlib are using counter clockwise angles in radiant.
        double fPhi_rad = Math::degree2Rad(Math::toCounterClockWiseAngleDegree(fPhi_degree));
        double fWidth = fabs(fDiameter * cos(fPhi_rad));
        double fHeight = fabs(fDiameter * sin(fPhi_rad));
        if (fPhi_degree >= 0.0 && fPhi_degree < 90.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 90.0));
        }
        else if (fPhi_degree >= 90.0 && fPhi_degree < 180.0) {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 180.0));
        }
        else if (fPhi_degree >= 180.0 && fPhi_degree < 270.0) {
            m_size.setWidth(fHeight);
            m_size.setHeight(fWidth);
            m_physValAngle.setVal(Math::normalizeAngleInDegree(m_physValAngle.getVal() + 270.0));
        }
        else /*if (fPhi_degree >= 270.0 && fPhi_degree < 360.0)*/ {
            m_size.setWidth(fWidth);
            m_size.setHeight(fHeight);
        }
    }
    m_arphysValRectSelectionPoints[static_cast<int>(ESelectionPoint::BottomLeft)] = physValPt;
    m_arbRectSelectionPointsCalculated[static_cast<int>(ESelectionPoint::BottomLeft)] = true;
    quint16 uSelectionPointsToExclude = 0;
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::BottomLeft));
    ZS::System::setBit(uSelectionPointsToExclude, static_cast<quint8>(ESelectionPoint::TopRight));
    invalidateSelectionPoints(uSelectionPointsToExclude);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a QRectF instance in the current unit of the unrotated rectangle.
*/
QRectF CPhysValRect::toQRectF() const
//------------------------------------------------------------------------------
{
    CPhysValRect physValRect = *this;
    physValRect.setAngle(0.0);
    QPointF ptTL = physValRect.topLeft().toQPointF();
    QSizeF size = physValRect.size().toQSizeF();
    return QRectF(ptTL, size);
}

//------------------------------------------------------------------------------
/*! @brief Returns a QRectF instance in the desired unit of the unrotated rectangle.
*/
QRectF CPhysValRect::toQRectF(const ZS::PhysVal::CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    CPhysValRect physValRect = *this;
    physValRect.setAngle(0.0);
    QPointF ptTL = physValRect.topLeft().toQPointF(i_unit);
    QSizeF size = physValRect.size().toQSizeF(i_unit);
    return QRectF(ptTL, size);
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical rectangle as a QPolygonF instance in the current unit.

    The points are returned in clockwise order starting with the top left corner.
*/
QPolygonF CPhysValRect::toQPolygonF() const
//------------------------------------------------------------------------------
{
    QPointF ptTL = topLeft().toQPointF();
    QPointF ptTR = topRight().toQPointF();
    QPointF ptBR = bottomRight().toQPointF();
    QPointF ptBL = bottomLeft().toQPointF();
    return QPolygonF({ptTL, ptTR, ptBR, ptBL});
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical rectangle as a QPolygonF instance in the desired unit.
*/
QPolygonF CPhysValRect::toQPolygonF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPointF ptTL = topLeft().toQPointF(i_unit);
    QPointF ptTR = topRight().toQPointF(i_unit);
    QPointF ptBR = bottomRight().toQPointF(i_unit);
    QPointF ptBL = bottomLeft().toQPointF(i_unit);
    return QPolygonF({ptTL, ptTR, ptBR, ptBL});
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Initializes the array with corner and other selection points and
           resets the calculated flags for each point.
*/
void CPhysValRect::initSelectionPoints()
//------------------------------------------------------------------------------
{
    m_arphysValRectSelectionPoints.clear();
    m_arbRectSelectionPointsCalculated.clear();
    // The first element is not used. This doesn't matter but simplifies the code.
    for (int selPt = 0; selPt <= ESelectionPointRectMax; ++selPt) {
        m_arphysValRectSelectionPoints.append(CPhysValPoint(*m_pDrawingScene));
        m_arbRectSelectionPointsCalculated.append(false);
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
            m_arphysValRectSelectionPoints[selPt] = CPhysValPoint(*m_pDrawingScene);
            m_arbRectSelectionPointsCalculated[selPt] = false;
        }
    }
}
