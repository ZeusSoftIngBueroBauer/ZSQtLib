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
    m_unit(),
    m_line()
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_line()
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    double i_fX1, double i_fY1, double i_fX2, double i_fY2, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_line(i_fX1, i_fY1, i_fX2, i_fY2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const QPointF& i_p1, const QPointF& i_p2, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_line(i_p1, i_p2)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const QLineF& i_line, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_line(i_line)
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(
    const CPhysValPoint& i_physValPoint1,
    const CPhysValPoint& i_physValPoint2) :
//------------------------------------------------------------------------------
    m_unit(i_physValPoint1.unit()),
    m_line(i_physValPoint1.toQPointF(m_unit), i_physValPoint2.toQPointF(m_unit))
{
    if (i_physValPoint1.unit() != i_physValPoint2.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine(const CPhysValLine& i_physValLineOther) :
//------------------------------------------------------------------------------
    m_unit(i_physValLineOther.m_unit),
    m_line(i_physValLineOther.m_line)
{
}

//------------------------------------------------------------------------------
CPhysValLine::~CPhysValLine()
//------------------------------------------------------------------------------
{
    //m_unit;
    //m_line;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine& CPhysValLine::operator = ( const CPhysValLine& i_physValLineOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValLineOther.m_unit;
    m_line = i_physValLineOther.m_line;
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
CUnit CPhysValLine::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::p1() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_line.p1(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::p2() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_line.p2(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x1(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x2(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y1(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y2(), m_unit);
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
    return CPhysVal(m_line.angleTo(i_physValLineOther.toQLineF(m_unit)), Units.Angle.Degree, 0.1);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValLine::center() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_line.center(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::dx() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.dx(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::dy() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.dy(), m_unit);
}

//------------------------------------------------------------------------------
QLineF::IntersectionType CPhysValLine::intersects(
    const CPhysValLine& i_physValLineOther, CPhysValPoint* i_physValPointIntersection) const
//------------------------------------------------------------------------------
{
    QPointF ptIntersection;
    QLineF::IntersectionType intersectionType = m_line.intersects(i_physValLineOther.toQLineF(m_unit), &ptIntersection);
    if (i_physValPointIntersection != nullptr) {
        *i_physValPointIntersection = CPhysValPoint(ptIntersection, m_unit);
    }
    return intersectionType;
}

//------------------------------------------------------------------------------
bool CPhysValLine::isNull() const
//------------------------------------------------------------------------------
{
    return m_line.isNull();
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::length() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.length(), m_unit);
}

//------------------------------------------------------------------------------
void CPhysValLine::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValLine::setP1( const CPhysValPoint& i_physValPoint1 )
//------------------------------------------------------------------------------
{
    m_line.setP1(QPointF(i_physValPoint1.x().getVal(m_unit), i_physValPoint1.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setP2( const CPhysValPoint& i_physValPoint2 )
//------------------------------------------------------------------------------
{
    m_line.setP2(QPointF(i_physValPoint2.x().getVal(m_unit), i_physValPoint2.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValLine::setAngle( const CPhysVal& i_physValAngle )
//------------------------------------------------------------------------------
{
    m_line.setAngle(i_physValAngle.getVal(Units.Angle.Degree));
}

//------------------------------------------------------------------------------
void CPhysValLine::setLength( const CPhysVal& i_physValLength )
//------------------------------------------------------------------------------
{
    m_line.setLength(i_physValLength.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValLine::setLine( double i_fX1, double i_fY1, double i_fX2, double i_fY2 )
//------------------------------------------------------------------------------
{
    m_line.setLine(i_fX1, i_fY1, i_fX2, i_fY2);
}

//------------------------------------------------------------------------------
void CPhysValLine::setPoints( const QPointF& i_p1, const QPointF& i_p2 )
//------------------------------------------------------------------------------
{
    m_line.setPoints(i_p1, i_p2);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical line as a QLineF instance in the desired unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
QLineF CPhysValLine::toQLineF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QLineF lineF = m_line;
    if (m_unit != i_unit) {
        double fX1_px = m_unit.convertValue(m_line.x1(), i_unit);
        double fY1_px = m_unit.convertValue(m_line.y1(), i_unit);
        double fX2_px = m_unit.convertValue(m_line.x2(), i_unit);
        double fY2_px = m_unit.convertValue(m_line.y2(), i_unit);
        lineF.setLine(fX1_px, fY1_px, fX2_px, fY2_px);
    }
    return lineF;
}

//------------------------------------------------------------------------------
QString CPhysValLine::toString() const
//------------------------------------------------------------------------------
{
    return x1().toString() + ", " + y1().toString() + ", " + x2().toString() + ", " + y2().toString();
}
