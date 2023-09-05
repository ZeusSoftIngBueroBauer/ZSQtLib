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

#include "ZSDraw/Common/ZSDrawPhysValSize.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_physValRes(i_resType),
    m_size()
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize( const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_size()
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize( const CPhysValSize& i_physValSizeOther ) :
//------------------------------------------------------------------------------
    m_unit(i_physValSizeOther.m_unit),
    m_physValRes(i_physValSizeOther.m_physValRes),
    m_size(i_physValSizeOther.m_size)
{
}

//------------------------------------------------------------------------------
CPhysValSize::~CPhysValSize()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValSize::operator == ( const CPhysValSize& i_physValSizeOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if (!areOfSameUnitGroup(m_unit, i_physValSizeOther.m_unit)) {
        bEqual = false;
    }
    else if (m_physValRes != i_physValSizeOther.m_physValRes) {
        bEqual = false;
    }
    else if (m_unit == i_physValSizeOther.m_unit && m_size != i_physValSizeOther.m_size) {
        bEqual = false;
    }
    else if (m_unit != i_physValSizeOther.m_unit && m_size == i_physValSizeOther.m_size) {
        bEqual = false;
    }
    else if (m_unit != i_physValSizeOther.m_unit && m_size != i_physValSizeOther.m_size) {
        double fWidth = i_physValSizeOther.width().getVal(m_unit);
        double fHeight = i_physValSizeOther.height().getVal(m_unit);
        if (fWidth != m_size.width() || fHeight != m_size.height()) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValSize::operator != ( const CPhysValSize& i_physValSizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValSizeOther);
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize& CPhysValSize::operator = ( const CPhysValSize& i_physValSizeOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValSizeOther.m_unit;
    m_physValRes = i_physValSizeOther.m_physValRes;
    m_size = i_physValSizeOther.m_size;
    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit CPhysValSize::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValSize::width() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_size.width(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValSize::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_size.height(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
bool CPhysValSize::isValid() const
//------------------------------------------------------------------------------
{
    return m_size.isValid();
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValSize::convertValues( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if (m_unit.isValid() && !areOfSameUnitGroup(m_unit, i_unitDst)) {
        QString strAddErrInfo = "Src: " + m_unit.keyInTree() + ", Dst: " + i_unitDst.keyInTree();
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo);
    }
    if (i_unitDst != m_unit) {
        double fWidth = m_size.width();
        fWidth = m_unit.convertValue(fWidth, i_unitDst);
        m_size.setWidth(fWidth);
        double fHeight = m_size.height();
        fHeight = m_unit.convertValue(fHeight, i_unitDst);
        m_size.setHeight(fHeight);
        m_physValRes.convertValue(i_unitDst);
    }
    m_unit = i_unitDst;
}
