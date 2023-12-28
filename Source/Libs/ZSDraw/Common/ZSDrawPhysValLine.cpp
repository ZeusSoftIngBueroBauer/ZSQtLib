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

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValLine
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine() :
//------------------------------------------------------------------------------
    m_line(),
    m_fRes(0.0),
    m_unit()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical line on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValLine::CPhysValLine(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    m_line(),
    m_fRes(i_drawingScene.drawingSize().imageCoorsResolution().getVal()),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CUnit& i_unit, double i_fRes) :
//------------------------------------------------------------------------------
    m_line(),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_line(i_fX1, i_fY1, i_fX2, i_fY2),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_line(i_p1, i_p2),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const QLineF& i_line, double i_fRes, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_line(i_line),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CPhysValPoint& i_physValP1,
    const CPhysValPoint& i_physValP2) :
//------------------------------------------------------------------------------
    m_line(i_physValP1.toQPointF(), i_physValP2.toQPointF()),
    m_fRes(i_physValP1.resolution()),
    m_unit(i_physValP1.unit())
{
    if (i_physValP1.unit() != i_physValP2.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValP2.resolution() > i_physValP2.resolution()) {
        m_fRes = i_physValP2.resolution();
    }
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CPhysValLine& i_physValLineOther) :
//------------------------------------------------------------------------------
    m_line(i_physValLineOther.m_line),
    m_fRes(i_physValLineOther.m_fRes),
    m_unit(i_physValLineOther.m_unit)
{
}

//------------------------------------------------------------------------------
CPhysValLine::~CPhysValLine()
//------------------------------------------------------------------------------
{
    //m_line;
    m_fRes = 0.0;
    //m_unit;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine& CPhysValLine::operator = ( const CPhysValLine& i_physValLineOther )
//------------------------------------------------------------------------------
{
    m_line = i_physValLineOther.m_line;
    m_fRes = i_physValLineOther.m_fRes;
    m_unit = i_physValLineOther.m_unit;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValLine& CPhysValLine::operator = ( const QLineF& i_line )
//------------------------------------------------------------------------------
{
    m_line = i_line;
    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValLine::operator == ( const CPhysValLine& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (!areOfSameUnitGroup(m_unit, i_physValLineOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit == i_physValLineOther.m_unit && m_line != i_physValLineOther.m_line) {
        bEqual = false;
    }
    else if (m_unit != i_physValLineOther.m_unit && m_line == i_physValLineOther.m_line) {
        bEqual = false;
    }
    else if (m_unit != i_physValLineOther.m_unit && m_line != i_physValLineOther.m_line) {
        double fX1 = i_physValLineOther.x1().getVal(m_unit);
        double fX2 = i_physValLineOther.x2().getVal(m_unit);
        double fY1 = i_physValLineOther.y1().getVal(m_unit);
        double fY2 = i_physValLineOther.y2().getVal(m_unit);
        if (fX1 != m_line.x1() || fX2 != m_line.x2() || fY1 != m_line.y1() || fY2 != m_line.y2()) {
            bEqual = false;
        }
        else if (m_fRes != i_physValLineOther.m_fRes) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValLine::operator != ( const CPhysValLine& i_physValLineOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValLineOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is valid, otherwise returns false.

    A valid rectangle has a width() > 0 and height() > 0.
*/
bool CPhysValLine::isValid() const
//------------------------------------------------------------------------------
{
    return !m_line.isNull();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::p1() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_line.p1(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::p2() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_line.p2(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x1(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x2(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y1(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y2(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::width() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.dx(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.dy(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::length() const
//------------------------------------------------------------------------------
{
    double fRes = m_fRes;
    if (m_unit == Units.Length.px) {
        fRes /= 10.0;
    }
    return CPhysVal(m_line.length(), m_unit, fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::angle() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.angle(), Units.Angle.Degree, 0.1);
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
CPhysValPoint CPhysValLine::center() const
//------------------------------------------------------------------------------
{
    double fRes = m_fRes;
    if (m_unit == Units.Length.px) {
        fRes /= 10.0;
    }
    return CPhysValPoint(m_line.center(), fRes, m_unit);
}

//------------------------------------------------------------------------------
double CPhysValLine::resolution() const
//------------------------------------------------------------------------------
{
    return m_fRes;
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
        *i_physValPointIntersection = CPhysValPoint(ptIntersection, m_fRes, m_unit);
    }
    return intersectionType;
}

//------------------------------------------------------------------------------
bool CPhysValLine::isNull() const
//------------------------------------------------------------------------------
{
    return m_line.isNull();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValLine::setP1( const CPhysValPoint& i_physValP1 )
//------------------------------------------------------------------------------
{
    m_line.setP1(QPointF(i_physValP1.x().getVal(m_unit), i_physValP1.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setP2( const CPhysValPoint& i_physValP2 )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(i_physValP2.x().getVal(m_unit), i_physValP2.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, double i_fRes, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line.setLine(i_fX1, i_fY1, i_fX2, i_fY2);
    m_fRes = i_fRes;
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setLine(const QLineF& i_line, double i_fRes, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line = i_line;
    m_fRes = i_fRes;
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setPoints(
    const QPointF& i_p1, const QPointF& i_p2, double i_fRes, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_line.setPoints(i_p1, i_p2);
    m_fRes = i_fRes;
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
    m_fRes = i_physValP1.resolution();
    m_unit = i_physValP1.unit();
}

//------------------------------------------------------------------------------
void CPhysValLine::setWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(m_line.x1() + i_physValWidth.getVal(m_unit), m_line.y2()));
}

//------------------------------------------------------------------------------
void CPhysValLine::setHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(m_line.x2(), m_line.y1() + i_physValHeight.getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setLength( const CPhysVal& i_physValLength )
//------------------------------------------------------------------------------
{
    m_line.setLength(i_physValLength.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValLine::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    m_line.setAngle(i_physValAngle.getVal(Units.Angle.Degree));
}

//------------------------------------------------------------------------------
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
void CPhysValLine::setResolution( double i_fRes )
//------------------------------------------------------------------------------
{
    m_fRes = i_fRes;
}

//------------------------------------------------------------------------------
void CPhysValLine::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
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
QString CPhysValLine::toString() const
//------------------------------------------------------------------------------
{
    return x1().toString() + ", " + y1().toString() + ", " + x2().toString() + ", " + y2().toString();
}
