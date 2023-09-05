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
CPhysValLine::CPhysValLine( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_physValRes(i_resType),
    m_line()
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine( const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_line()
{
}

//------------------------------------------------------------------------------
CPhysValLine::CPhysValLine( const CPhysValLine& i_physValLineOther ) :
//------------------------------------------------------------------------------
    m_unit(i_physValLineOther.m_unit),
    m_physValRes(i_physValLineOther.m_physValRes),
    m_line(i_physValLineOther.m_line)
{
}

//------------------------------------------------------------------------------
CPhysValLine::~CPhysValLine()
//------------------------------------------------------------------------------
{
} // dtor

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
    else if (m_physValRes != i_physValLineOther.m_physValRes) {
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
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValLine& CPhysValLine::operator = ( const CPhysValLine& i_physValLineOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValLineOther.m_unit;
    m_physValRes = i_physValLineOther.m_physValRes;
    m_line = i_physValLineOther.m_line;
    return *this;
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
CPhysVal CPhysValLine::x1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x1(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::x2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.x2(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y1() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y1(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValLine::y2() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_line.y2(), m_unit, m_physValRes);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValLine::convertValues( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if (m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unitDst)) {
        QString strAddErrInfo = "Src: " + m_unit.keyInTree() + ", Dst: " + i_unitDst.keyInTree();
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo);
    }
    if (i_unitDst != m_unit) {
        double fX1 = m_line.x1();
        fX1 = m_unit.convertValue(fX1, i_unitDst);
        double fY1 = m_line.y1();
        fY1 = m_unit.convertValue(fY1, i_unitDst);
        m_line.setP1(QPointF(fX1, fY1));
        double fX2 = m_line.x2();
        fX2 = m_unit.convertValue(fX2, i_unitDst);
        double fY2 = m_line.y2();
        fY2 = m_unit.convertValue(fY2, i_unitDst);
        m_line.setP1(QPointF(fX2, fY2));
        m_physValRes.convertValue(i_unitDst);
    }
    m_unit = i_unitDst;
}
