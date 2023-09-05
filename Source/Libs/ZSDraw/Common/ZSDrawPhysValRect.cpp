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
#include "ZSPhysVal/ZSPhysValExceptions.h"

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
CPhysValRect::CPhysValRect( EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(),
    m_physValRes(i_resType),
    m_rect()
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect( const CUnit& i_unit, double i_fRes, EResType i_resType ) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_physValRes(i_fRes, i_unit, i_resType),
    m_rect()
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect( const CPhysValRect& i_physValRectOther ) :
//------------------------------------------------------------------------------
    m_unit(i_physValRectOther.m_unit),
    m_physValRes(i_physValRectOther.m_physValRes),
    m_rect(i_physValRectOther.m_rect)
{
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
} // dtor

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
    else if (m_physValRes != i_physValRectOther.m_physValRes) {
        bEqual = false;
    }
    else if (m_unit == i_physValRectOther.m_unit && m_rect != i_physValRectOther.m_rect) {
        bEqual = false;
    }
    else if (m_unit != i_physValRectOther.m_unit && m_rect == i_physValRectOther.m_rect) {
        bEqual = false;
    }
    else if (m_unit != i_physValRectOther.m_unit && m_rect != i_physValRectOther.m_rect) {
        double fTop = i_physValRectOther.top().getVal(m_unit);
        double fBottom = i_physValRectOther.bottom().getVal(m_unit);
        double fWidth = i_physValRectOther.width().getVal(m_unit);
        double fHeight = i_physValRectOther.height().getVal(m_unit);
        if (fTop != m_rect.top() || fBottom != m_rect.bottom()
         || fWidth != m_rect.width() || fHeight != m_rect.height()) {
            bEqual = false;
        }
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
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const CPhysValRect& i_physValRectOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValRectOther.m_unit;
    m_physValRes = i_physValRectOther.m_physValRes;
    m_rect = i_physValRectOther.m_rect;
    return *this;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CUnit CPhysValRect::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::top() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.top(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::bottom() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.bottom(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.width(), m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.height(), m_unit, m_physValRes);
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRect::convertValues( const CUnit& i_unitDst )
//------------------------------------------------------------------------------
{
    if (m_unit.isValid() && !areOfSameUnitGroup(m_unit,i_unitDst)) {
        QString strAddErrInfo = "Src: " + m_unit.keyInTree() + ", Dst: " + i_unitDst.keyInTree();
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes, strAddErrInfo);
    }
    if (i_unitDst != m_unit) {
        double fTop = m_rect.top();
        fTop = m_unit.convertValue(fTop, i_unitDst);
        m_rect.setTop(fTop);
        double fBottom = m_rect.bottom();
        fBottom = m_unit.convertValue(fBottom, i_unitDst);
        m_rect.setBottom(fTop);
        double fWidth = m_rect.width();
        fWidth = m_unit.convertValue(fWidth, i_unitDst);
        m_rect.setWidth(fWidth);
        double fHeight = m_rect.height();
        fHeight = m_unit.convertValue(fHeight, i_unitDst);
        m_rect.setHeight(fHeight);
        m_physValRes.convertValue(i_unitDst);
    }
    m_unit = i_unitDst;
}
