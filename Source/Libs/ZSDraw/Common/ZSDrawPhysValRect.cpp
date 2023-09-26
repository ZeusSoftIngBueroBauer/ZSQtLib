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
#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
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
CPhysValRect::CPhysValRect() :
//------------------------------------------------------------------------------
    m_unit(),
    m_rect()
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_rect()
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const QRectF& i_rect, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_unit(i_unit),
    m_rect(i_rect)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CPhysValPoint& i_physValTopLeft, const CPhysValPoint& i_physValBottomRight) :
//------------------------------------------------------------------------------
    m_unit(i_physValTopLeft.unit()),
    m_rect(i_physValTopLeft.toQPointF(m_unit), i_physValBottomRight.toQPointF(m_unit))
{
    if (i_physValTopLeft.unit() != i_physValBottomRight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    m_unit(i_physValRectOther.m_unit),
    m_rect(i_physValRectOther.m_rect)
{
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    //m_unit;
    //m_rect;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const CPhysValRect& i_physValRectOther )
//------------------------------------------------------------------------------
{
    m_unit = i_physValRectOther.m_unit;
    m_rect = i_physValRectOther.m_rect;
    return *this;
}

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const QRectF& i_rect )
//------------------------------------------------------------------------------
{
    m_rect = i_rect;
    return *this;
}

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
    return CPhysVal(m_rect.top(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::bottom() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.bottom(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::left() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.left(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::right() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.right(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.width(), m_unit);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.height(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.left(), m_rect.top());
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.right(), m_rect.top());
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.left(), m_rect.bottom());
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.right(), m_rect.bottom());
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRect::setUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    m_unit = i_unit;
}

/*==============================================================================
public: // instance methods (to convert the values into another unit)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the physical line as a QRectF instance in the desired unit.

    @param [in] i_unit
        Unit in which the coordinates should be returned.
*/
QRectF CPhysValRect::toQRectF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    QRectF rectF = m_rect;
    if (m_unit != i_unit) {
        double fLeft = m_unit.convertValue(m_rect.left(), i_unit);
        double fTop = m_unit.convertValue(m_rect.top(), i_unit);
        double fWidth = m_unit.convertValue(m_rect.width(), i_unit);
        double fHeight = m_unit.convertValue(m_rect.height(), i_unit);
        rectF.setRect(fLeft, fTop, fWidth, fHeight);
    }
    return rectF;
}

//------------------------------------------------------------------------------
QString CPhysValRect::toString() const
//------------------------------------------------------------------------------
{
    return left().toString() + ", " + top().toString() + ", " + width().toString() + ", " + height().toString();
}
