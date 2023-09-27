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
#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
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
CPhysValSize::CPhysValSize() :
//------------------------------------------------------------------------------
    m_unit(),
    m_size()
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_size()
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(double i_fWidth, double i_fHeight, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_size(i_fWidth, i_fHeight)
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(const QSizeF& i_size, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_size(i_size)
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight) :
//------------------------------------------------------------------------------
    m_unit(i_physValWidth.unit()),
    m_size(i_physValWidth.getVal(), i_physValHeight.getVal())
{
    if (i_physValWidth.unit() != i_physValHeight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(const CPhysValSize& i_physValSizeOther) :
//------------------------------------------------------------------------------
    m_unit(i_physValSizeOther.m_unit),
    m_size(i_physValSizeOther.m_size)
{
}

//------------------------------------------------------------------------------
CPhysValSize::~CPhysValSize()
//------------------------------------------------------------------------------
{
    //m_unit;
    //m_size;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize& CPhysValSize::operator = ( const CPhysValSize& i_physValSizeOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValSizeOther.m_unit;
    m_size = i_physValSizeOther.m_size;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValSize& CPhysValSize::operator = ( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    m_size = i_size;
    return *this;
}

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
    return CPhysVal(m_size.width(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValSize::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_size.height(), m_unit);
}

//------------------------------------------------------------------------------
bool CPhysValSize::isValid() const
//------------------------------------------------------------------------------
{
    return m_size.isValid();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValSize::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
}

//------------------------------------------------------------------------------
void CPhysValSize::setWidth( const CPhysVal& i_physValWidth )
//------------------------------------------------------------------------------
{
    m_size.setWidth(i_physValWidth.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValSize::setHeight( const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    m_size.setHeight(i_physValHeight.getVal(m_unit));
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical size as a QSizeF instance.
*/
QSizeF CPhysValSize::toQSizeF() const
//------------------------------------------------------------------------------
{
    return m_size;
}

//------------------------------------------------------------------------------
QString CPhysValSize::toString() const
//------------------------------------------------------------------------------
{
    return width().toString() + ", " + height().toString();
}
