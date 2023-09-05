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
CPhysValPoint::CPhysValPoint( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_physValRes(i_resType),
    m_pt()
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint( const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_pt()
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint( const CPhysValPoint& i_physValPointOther ) :
//------------------------------------------------------------------------------
    m_unit(i_physValPointOther.m_unit),
    m_physValRes(i_physValPointOther.m_physValRes),
    m_pt(i_physValPointOther.m_pt)
{
}

//------------------------------------------------------------------------------
CPhysValPoint::~CPhysValPoint()
//------------------------------------------------------------------------------
{
} // dtor

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
    else if (m_physValRes != i_physValPointOther.m_physValRes) {
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
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint& CPhysValPoint::operator = ( const CPhysValPoint& i_physValPointOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValPointOther.m_unit;
    m_physValRes = i_physValPointOther.m_physValRes;
    m_pt = i_physValPointOther.m_pt;
    return *this;
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
    return CPhysVal(m_pt.x(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPoint::y() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_pt.y(), m_unit, m_physValRes);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValPoint::convertValues( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if (m_unit.isValid() && !areOfSameUnitGroup(m_unit, i_unitDst)) {
        QString strAddErrInfo = "Src: " + m_unit.keyInTree() + ", Dst: " + i_unitDst.keyInTree();
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo);
    }
    if (i_unitDst != m_unit) {
        double fX = m_pt.x();
        fX = m_unit.convertValue(fX, i_unitDst);
        m_pt.setX(fX);
        double fY = m_pt.y();
        fY = m_unit.convertValue(fY, i_unitDst);
        m_pt.setY(fY);
        m_physValRes.convertValue(i_unitDst);
    }
    m_unit = i_unitDst;
}
