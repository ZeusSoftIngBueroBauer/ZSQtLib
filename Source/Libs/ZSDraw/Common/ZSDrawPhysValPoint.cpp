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

#include "ZSDraw/Common/ZSDrawPhysValPoint.h"
#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValPoint
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint() :
//------------------------------------------------------------------------------
    m_unit(),
    m_pt()
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint(const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_pt()
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(const QPointF& i_pt, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_pt(i_pt)
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint(const CPhysVal& i_physValX, const CPhysVal& i_physValY) :
//------------------------------------------------------------------------------
    m_unit(i_physValX.unit()),
    m_pt(i_physValX.getVal(), i_physValY.getVal())
{
    if (i_physValX.unit() != i_physValY.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint( const CPhysValPoint& i_physValPointOther ) :
//------------------------------------------------------------------------------
    m_unit(i_physValPointOther.m_unit),
    m_pt(i_physValPointOther.m_pt)
{
}

//------------------------------------------------------------------------------
CPhysValPoint::~CPhysValPoint()
//------------------------------------------------------------------------------
{
    //m_unit;
    //m_pt;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint& CPhysValPoint::operator = ( const CPhysValPoint& i_physValPointOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValPointOther.m_unit;
    m_pt = i_physValPointOther.m_pt;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValPoint& CPhysValPoint::operator = ( const QPointF& i_ptOther )
//------------------------------------------------------------------------------
{
    m_pt = i_ptOther;
    return *this;
}

//------------------------------------------------------------------------------
/*! @brief Assigns the string containing a physical point definition to this.

    @param [in] i_strValOther
        String containing the physical point definition in the following format:
        "X [unit] / Y [unit]".
        If [unit] is omitted the current unit of the instance is kept unchanged.
        If no current unit has been assigned to this yet, the unit "px" is used.

    @return New physical point value.

    @note throws a CUnitConversionException if conversion fails.
*/
CPhysValPoint& CPhysValPoint::operator = ( const QString& i_strValOther )
//------------------------------------------------------------------------------
{
    QStringList strlst = i_strValOther.split("/");
    if (strlst.size() != 2) {
        throw CUnitConversionException(
            __FILE__, __LINE__, EResultArgOutOfRange, i_strValOther);
    }

    CPhysVal physValX(m_unit);
    CPhysVal physValY(m_unit);
    physValX = strlst[0];
    physValY = strlst[1];
    if (!m_unit.isValid()) {
        if (physValX.unit().isValid()) {
            m_unit = physValX.unit();
        }
        else if (physValY.unit().isValid()) {
            m_unit = physValY.unit();
        }
        else {
            m_unit = Units.Length.px;
        }
    }
    physValX.convertValue(m_unit);
    physValY.convertValue(m_unit);
    m_pt.setX(physValX.getVal());
    m_pt.setY(physValY.getVal());
    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValPoint::operator == ( const CPhysValPoint& i_physValPointOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (!areOfSameUnitGroup(m_unit, i_physValPointOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit == i_physValPointOther.m_unit && m_pt != i_physValPointOther.m_pt) {
        bEqual = false;
    }
    else if (m_unit != i_physValPointOther.m_unit && m_pt == i_physValPointOther.m_pt) {
        bEqual = false;
    }
    else if (m_unit != i_physValPointOther.m_unit && m_pt != i_physValPointOther.m_pt) {
        double fXOther = i_physValPointOther.x().getVal(m_unit);
        double fYOther = i_physValPointOther.y().getVal(m_unit);
        if (fXOther != m_pt.x() || fYOther != m_pt.y()) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValPoint::operator != ( const CPhysValPoint& i_physValPointOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValPointOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit CPhysValPoint::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPoint::x() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_pt.x(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPoint::y() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_pt.y(), m_unit);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValPoint::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValPoint::setX( const CPhysVal& i_physValX )
//------------------------------------------------------------------------------
{
    m_pt.setX(i_physValX.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValPoint::setY( const CPhysVal& i_physValY )
//------------------------------------------------------------------------------
{
    m_pt.setY(i_physValY.getVal(m_unit));
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical point as a QPointF instance in the desired unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
QPointF CPhysValPoint::toQPointF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QPointF ptF = m_pt;
    if (m_unit != i_unit) {
        double fX = m_unit.convertValue(m_pt.x(), i_unit);
        double fY = m_unit.convertValue(m_pt.y(), i_unit);
        ptF.setX(fX);
        ptF.setY(fY);
    }
    return ptF;
}

//------------------------------------------------------------------------------
QString CPhysValPoint::toString() const
//------------------------------------------------------------------------------
{
    return x().toString() + ", " + y().toString();
}
