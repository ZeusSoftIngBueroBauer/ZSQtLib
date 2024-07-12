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

#include "ZSDraw/Common/ZSDrawPhysValLine.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValLine
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical line on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValLine::CPhysValLine(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_line()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical line on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValLine::CPhysValLine(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_line()
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    double i_fX1, double i_fY1, double i_fX2, double i_fY2) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_line(i_fX1, i_fY1, i_fX2, i_fY2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    double i_fX1, double i_fY1, double i_fX2, double i_fY2,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_line(i_fX1, i_fY1, i_fX2, i_fY2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_p1, const QPointF& i_p2) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_line(i_p1, i_p2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_p1, const QPointF& i_p2,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_line(i_p1, i_p2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    const QLineF& i_line) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_line(i_line)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CDrawingScene& i_drawingScene,
    const QLineF& i_line,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_line(i_line)
{
}

///*==============================================================================
//public: // ctors
//==============================================================================*/
//
////------------------------------------------------------------------------------
///*! @brief Creates a physical line on the drawing scene in the current unit
//           and current resolution of the drawing scene.
//*/
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_line()
//{
//}
//
////------------------------------------------------------------------------------
///*! @brief Creates a physical line on the drawing scene in the current unit
//           and current resolution of the drawing scene.
//*/
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_line()
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    double i_fX1, double i_fY1, double i_fX2, double i_fY2) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_line(i_fX1, i_fY1, i_fX2, i_fY2)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    double i_fX1, double i_fY1, double i_fX2, double i_fY2,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_line(i_fX1, i_fY1, i_fX2, i_fY2)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QPointF& i_p1, const QPointF& i_p2) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_line(i_p1, i_p2)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QPointF& i_p1, const QPointF& i_p2,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_line(i_p1, i_p2)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QLineF& i_line) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_line(i_line)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValLine::CPhysValLine(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QLineF& i_line,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_line(i_line)
//{
//}

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValP1.drawingScene(), i_physValP1.unit()),
    m_line(i_physValP1.toQPointF(), i_physValP2.toQPointF())
{
    if (&i_physValP1.drawingScene() != &i_physValP2.drawingScene()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    //if (&i_physValP1.divLinesMetricsX() != &i_physValP2.divLinesMetricsX()) {
    //    throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    //}
    //if (&i_physValP1.divLinesMetricsY() != &i_physValP2.divLinesMetricsY()) {
    //    throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    //}
    if (i_physValP1.unit() != i_physValP2.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValP1.resolution() != i_physValP2.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CPhysValLine& i_physValLineOther) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValLineOther),
    m_line(i_physValLineOther.m_line)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine::~CPhysValLine()
//------------------------------------------------------------------------------
{
    //m_line;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine& CPhysValLine::operator = ( const QLineF& i_line )
//------------------------------------------------------------------------------
{
    m_line = i_line;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValLine::operator = ( const CPhysValShape& i_physValLineOther )
//------------------------------------------------------------------------------
{
    const CPhysValLine& physValLineOther = dynamic_cast<const CPhysValLine&>(i_physValLineOther);
    //m_pDivLinesMetricsX = physValLineOther.m_pDivLinesMetricsX;
    //m_pDivLinesMetricsY = physValLineOther.m_pDivLinesMetricsY;
    m_unit = physValLineOther.m_unit;
    m_line = physValLineOther.m_line;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValLine::operator == ( const CPhysValShape& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    const CPhysValLine& physValLineOther = dynamic_cast<const CPhysValLine&>(i_physValLineOther);
    if (!areOfSameUnitGroup(m_unit, physValLineOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit == physValLineOther.m_unit && m_line != physValLineOther.m_line) {
        bEqual = false;
    }
    else if (m_unit != physValLineOther.m_unit && m_line == physValLineOther.m_line) {
        bEqual = false;
    }
    else if (m_unit != physValLineOther.m_unit && m_line != physValLineOther.m_line) {
        double fX1 = physValLineOther.x1().getVal(m_unit);
        double fX2 = physValLineOther.x2().getVal(m_unit);
        double fY1 = physValLineOther.y1().getVal(m_unit);
        double fY2 = physValLineOther.y2().getVal(m_unit);
        if (fX1 != m_line.x1() || fX2 != m_line.x2() || fY1 != m_line.y1() || fY2 != m_line.y2()) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValLine::operator != ( const CPhysValShape& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValLineOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the coordinates by setting both the start and point to (0.0, 0.0).
*/
void CPhysValLine::invalidate()
//------------------------------------------------------------------------------
{
    m_line = QLineF(QPointF(0.0, 0.0), QPointF(0.0, 0.0));
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the line is valid, otherwise returns false.

    A line is always valid, even if it does not have distinct start and end points.
*/
bool CPhysValLine::isValid() const
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the line is valid, otherwise returns false.

    A line is invalid if it does not have distinct start and end points.
    The start and end points are considered distinct if qFuzzyCompare()
    can distinguish them in at least one coordinate.
*/
bool CPhysValLine::isNull() const
//------------------------------------------------------------------------------
{
    return m_line.isNull();
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValLine::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
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
QString CPhysValLine::toString(bool i_bAddUnit, const QString& i_strSeparator, int i_iPrecision) const
//------------------------------------------------------------------------------
{
    QString str;
    if (i_iPrecision < 0) {
        str = x1().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + y1().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + x2().toString(EUnitFind::None, PhysValSubStr::Val)
            + i_strSeparator
            + y2().toString(EUnitFind::None, PhysValSubStr::Val);
    }
    else {
        str = QString::number(x1().getVal(), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(y1().getVal(), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(x2().getVal(), 'f', i_iPrecision)
            + i_strSeparator
            + QString::number(y2().getVal(), 'f', i_iPrecision);
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
CPhysValPoint CPhysValLine::p1() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_line.p1(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::p2() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_line.p2(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x1() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.x1(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x2() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.x2(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y1() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.y1(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y2() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.y2(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::dx() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.dx(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::dy() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.dy(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::length() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_line.length(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::center() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_line.center(), m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the angle in degrees.

    The angles are measured clockwise around the z-Axis through point P1.

    If the yScaleAxisOrientation is from BottomToTop the angle must be corrected
    correspondingly.
*/
CPhysVal CPhysValLine::angle() const
//------------------------------------------------------------------------------
{
    // QLineF::angle returns the angle counter-clockwise which is in contrast to
    // the QGraphicsItem::rotationAngle. This will be corrected here.
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValAngle(m_line.angle(), Units.Angle.Degree, 0.1);
    if (drawingSize.yScaleAxisOrientation() != EYScaleAxisOrientation::BottomUp) {
        double fAngle_degree = Math::normalizeAngleInDegree(360.0 - physValAngle.getVal());
        physValAngle.setVal(fAngle_degree);
    }
    return physValAngle;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::angleTo(const CPhysValLine& i_physValLineOther) const
//------------------------------------------------------------------------------
{
    if (m_unit != i_physValLineOther.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    return CPhysVal(m_line.angleTo(i_physValLineOther.toQLineF()), Units.Angle.Degree, 0.1);
}

//------------------------------------------------------------------------------
double CPhysValLine::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
}

//------------------------------------------------------------------------------
CUnit CPhysValLine::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
QLineF::IntersectionType CPhysValLine::intersects(
    const CPhysValLine& i_physValLineOther, CPhysValPoint* i_physValPointIntersection) const
//------------------------------------------------------------------------------
{
    if (m_unit != i_physValLineOther.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    QPointF ptIntersection;
    QLineF::IntersectionType intersectionType = m_line.intersects(i_physValLineOther.toQLineF(), &ptIntersection);
    if (i_physValPointIntersection != nullptr) {
        *i_physValPointIntersection = CPhysValPoint(*m_pDrawingScene, ptIntersection, m_unit);
    }
    return intersectionType;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValLine::setP1( const QPointF& i_p1 )
//------------------------------------------------------------------------------
{
    m_line.setP1(i_p1);
}

//------------------------------------------------------------------------------
void CPhysValLine::setP1( const CPhysValPoint& i_physValP1 )
//------------------------------------------------------------------------------
{
    m_line.setP1(QPointF(i_physValP1.x().getVal(m_unit), i_physValP1.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setP2( const QPointF& i_p2 )
//------------------------------------------------------------------------------
{
    m_line.setP2(i_p2);
}

//------------------------------------------------------------------------------
void CPhysValLine::setP2( const CPhysValPoint& i_physValP2 )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(i_physValP2.x().getVal(m_unit), i_physValP2.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line.setLine(i_fX1, i_fY1, i_fX2, i_fY2);
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setLine(const QLineF& i_line, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line = i_line;
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setPoints(
    const QPointF& i_p1, const QPointF& i_p2, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line.setPoints(i_p1, i_p2);
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setPoints(
    const CPhysValPoint& i_physValP1, const CPhysValPoint& i_physValP2)
//------------------------------------------------------------------------------
{
    if (i_physValP1.unit() != i_physValP2.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValP1.resolution() != i_physValP2.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    m_line.setPoints(i_physValP1.toQPointF(), i_physValP2.toQPointF());
    m_unit = i_physValP1.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the horizontal component of the lines vector by moving p2.

    @param [in] i_fDX The value has to be provided in the current unit of the line.

    |<- DX ->|
    x--------x
    p1      p2
*/
void CPhysValLine::setDX( double i_fDX )
//------------------------------------------------------------------------------
{
    setDX( CPhysVal(i_fDX, m_unit) );
}

//------------------------------------------------------------------------------
/*! @brief Sets the horizontal component of the lines vector by moving p2.

    |<- DX ->|
    x--------x
    p1      p2
*/
void CPhysValLine::setDX( const CPhysVal& i_physValDX )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(m_line.x1() + i_physValDX.getVal(m_unit), m_line.y2()));
}

//------------------------------------------------------------------------------
/*! @brief Sets the vertical component of the lines vector by moving p2.

    @param [in] i_fDY The value has to be provided in the current unit of the line.

    x p1 --
    |     ^
    |     DY
    |     
    x p2 --
*/
void CPhysValLine::setDY( double i_fDY )
//------------------------------------------------------------------------------
{
    setDY( CPhysVal(i_fDY, m_unit) );
}

//------------------------------------------------------------------------------
/*! @brief Sets the vertical component of the lines vector by moving p2.

    x p1 --
    |     ^
    |     DY
    |     
    x p2 --
*/
void CPhysValLine::setDY( const CPhysVal& i_physValDY )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(m_line.x2(), m_line.y1() + i_physValDY.getVal(m_unit)));
}

//------------------------------------------------------------------------------
/*! @brief Sets the length of the line to the given length.

    The end point (p2) of the line is moved to give the line its new length,
    unless length() was previously zero, in which case no scaling is attempted.
*/
void CPhysValLine::setLength( double i_fLength )
//------------------------------------------------------------------------------
{
    m_line.setLength(i_fLength);
}

//------------------------------------------------------------------------------
/*! @brief Sets the length of the line to the given length.

    The end point (p2) of the line is moved to give the line its new length,
    unless length() was previously zero, in which case no scaling is attempted.
*/
void CPhysValLine::setLength( const CPhysVal& i_physValLength )
//------------------------------------------------------------------------------
{
    m_line.setLength(i_physValLength.getVal(m_unit));
}

//------------------------------------------------------------------------------
/*! @brief Moves the center point of the line by keeping the length of the line.

    The start and end points of the line are moved to keep the length of the line.
*/
void CPhysValLine::setCenter(const QPointF& i_pCenter)
//------------------------------------------------------------------------------
{
    double fCenterX = i_pCenter.x();
    double fdx = fCenterX - m_line.center().x();
    double fCenterY = i_pCenter.y();
    double fdy = fCenterY - m_line.center().y();
    m_line.translate(fdx, fdy);
}

//------------------------------------------------------------------------------
/*! @brief Moves the center point of the line by keeping the length of the line.

    The start and end points of the line are moved to keep the length of the line.
*/
void CPhysValLine::setCenter(const CPhysValPoint& i_physValPointCenter)
//------------------------------------------------------------------------------
{
    double fCenterX = i_physValPointCenter.x().getVal(m_unit);
    double fdx = fCenterX - m_line.center().x();
    double fCenterY = i_physValPointCenter.y().getVal(m_unit);
    double fdy = fCenterY - m_line.center().y();
    m_line.translate(fdx, fdy);
}

//------------------------------------------------------------------------------
/*! @brief Sets the angle of the line in the given unit.

    The center point and the length of the line remains unchanged.
    Points 1 and 2 are moved correspondingly.

    The angles are measured clockwise around the z-Axis through point P1.

    If the yScaleAxisOrientation is from BottomToTop the angle must be corrected
    correspondingly.

    @param [in] i_physValAngle
        Angle to be set.
*/
void CPhysValLine::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    // QLineF::angle returns the angle counter-clockwise which is in contrast to
    // the QGraphicsItem::rotationAngle. This will be corrected here.
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    QPointF ptCenter = m_line.center();
    CPhysVal physValAngle = i_physValAngle;
    if (drawingSize.yScaleAxisOrientation() != EYScaleAxisOrientation::BottomUp) {
        double fAngle_degree = Math::normalizeAngleInDegree(360.0 - physValAngle.getVal());
        physValAngle.setVal(fAngle_degree);
    }
    double fAngle_rad = physValAngle.getVal(Units.Angle.Rad);
    double fRadius = m_line.length() / 2.0;
    double dx = fRadius * cos(fAngle_rad);
    double dy = fRadius * sin(fAngle_rad);
    m_line.setP1(ptCenter + QPointF(-dx, dy));
    m_line.setP2(ptCenter + QPointF(dx, -dy));
    //if (m_line.center() != ptCenter) {
    //    throw CException(__FILE__, __LINE__, EResultInternalProgramError);
    //}
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical line as a QLineF instance.
*/
QLineF CPhysValLine::toQLineF() const
//------------------------------------------------------------------------------
{
    return m_line;
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical point as a QPointF instance in the desired unit.
* 
*/
QLineF CPhysValLine::toQLineF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QLineF lineF = m_line;
    if (i_unit != m_unit) {
        QPointF pt1(m_line.p1());
        pt1.setX(CPhysVal(pt1.x(), m_unit).getVal(i_unit));
        pt1.setY(CPhysVal(pt1.y(), m_unit).getVal(i_unit));
        QPointF pt2(m_line.p2());
        pt2.setX(CPhysVal(pt2.x(), m_unit).getVal(i_unit));
        pt2.setY(CPhysVal(pt2.y(), m_unit).getVal(i_unit));
        lineF.setP1(pt1);
        lineF.setP2(pt2);
    }
    return lineF;
}
