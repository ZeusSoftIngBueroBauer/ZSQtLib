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
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
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
    m_rect(),
    m_fRes(0.0),
    m_unit()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical rectangle on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    m_rect(),
    m_fRes(i_drawingScene.drawingSize().imageCoorsResolution().getVal()),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CUnit& i_unit, double i_fRes) :
//------------------------------------------------------------------------------
    m_rect(),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const QRectF& i_rect, double i_fRes, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_rect(i_rect),
    m_fRes(i_fRes),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CPhysValPoint& i_physValTopLeft, const CPhysValPoint& i_physValBottomRight) :
//------------------------------------------------------------------------------
    m_rect(i_physValTopLeft.toQPointF(), i_physValBottomRight.toQPointF()),
    m_fRes(i_physValTopLeft.resolution()),
    m_unit(i_physValTopLeft.unit())
{
    if (i_physValTopLeft.unit() != i_physValBottomRight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValBottomRight.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    m_rect(i_physValRectOther.m_rect),
    m_fRes(i_physValRectOther.m_fRes),
    m_unit(i_physValRectOther.m_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    //m_rect;
    m_fRes = 0.0;
    //m_unit;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValRect& CPhysValRect::operator = ( const CPhysValRect& i_physValRectOther )
//------------------------------------------------------------------------------
{
    m_rect = i_physValRectOther.m_rect;
    m_fRes = i_physValRectOther.m_fRes;
    m_unit = i_physValRectOther.m_unit;
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
        else if (m_fRes != i_physValRectOther.m_fRes) {
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
/*! @brief Returns true if the rectangle is valid, otherwise returns false.

    A valid rectangle has a width() > 0 and height() > 0.
*/
bool CPhysValRect::isValid() const
//------------------------------------------------------------------------------
{
    return m_rect.isValid();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::top() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.top(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::bottom() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.bottom(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::left() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.left(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::right() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.right(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.width(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::height() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_rect.height(), m_unit, m_fRes);
}

//------------------------------------------------------------------------------
CPhysValSize CPhysValRect::size() const
//------------------------------------------------------------------------------
{
    return CPhysValSize(m_rect.size(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::center() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.center(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.left(), m_rect.top(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.right(), m_rect.top(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.left(), m_rect.bottom(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(m_rect.right(), m_rect.bottom(), m_fRes, m_unit);
}

//------------------------------------------------------------------------------
double CPhysValRect::resolution() const
//------------------------------------------------------------------------------
{
    return m_fRes;
}

//------------------------------------------------------------------------------
CUnit CPhysValRect::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CPhysValRect::setTop(double i_fTop)
//------------------------------------------------------------------------------
{
    m_rect.setTop(i_fTop);
}

//------------------------------------------------------------------------------
void CPhysValRect::setTop(const ZS::PhysVal::CPhysVal& i_physValTop)
//------------------------------------------------------------------------------
{
    m_rect.setTop(i_physValTop.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottom(double i_fBottom)
//------------------------------------------------------------------------------
{
    m_rect.setBottom(i_fBottom);
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottom(const ZS::PhysVal::CPhysVal& i_physValBottom)
//------------------------------------------------------------------------------
{
    m_rect.setBottom(i_physValBottom.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setLeft(double i_fLeft)
//------------------------------------------------------------------------------
{
    m_rect.setLeft(i_fLeft);
}

//------------------------------------------------------------------------------
void CPhysValRect::setLeft(const ZS::PhysVal::CPhysVal& i_physValLeft)
//------------------------------------------------------------------------------
{
    m_rect.setLeft(i_physValLeft.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setRight(double i_fRight)
//------------------------------------------------------------------------------
{
    m_rect.setRight(i_fRight);
}

//------------------------------------------------------------------------------
void CPhysValRect::setRight(const ZS::PhysVal::CPhysVal& i_physValRight)
//------------------------------------------------------------------------------
{
    m_rect.setRight(i_physValRight.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setWidth(double i_fWidth)
//------------------------------------------------------------------------------
{
    m_rect.setWidth(i_fWidth);
}

//------------------------------------------------------------------------------
void CPhysValRect::setWidth(const ZS::PhysVal::CPhysVal& i_physValWidth)
//------------------------------------------------------------------------------
{
    m_rect.setWidth(i_physValWidth.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setHeight(double i_fHeight)
//------------------------------------------------------------------------------
{
    m_rect.setHeight(i_fHeight);
}

//------------------------------------------------------------------------------
void CPhysValRect::setHeight(const ZS::PhysVal::CPhysVal& i_physValHeight)
//------------------------------------------------------------------------------
{
    m_rect.setHeight(i_physValHeight.getVal(m_unit));
}

//------------------------------------------------------------------------------
void CPhysValRect::setSize(const QSizeF& i_size)
//------------------------------------------------------------------------------
{
    m_rect.setSize(i_size);
}

//------------------------------------------------------------------------------
void CPhysValRect::setSize(const CPhysValSize& i_physValSize)
//------------------------------------------------------------------------------
{
    m_rect.setSize(QSizeF(i_physValSize.width().getVal(m_unit), i_physValSize.height().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::moveCenter(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_rect.moveCenter(i_pt);
}

//------------------------------------------------------------------------------
void CPhysValRect::moveCenter(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_rect.moveCenter(QPointF(i_physValPoint.x().getVal(m_unit), i_physValPoint.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::setTopLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_rect.setTopLeft(i_pt);
}

//------------------------------------------------------------------------------
void CPhysValRect::setTopLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_rect.setTopLeft(QPointF(i_physValPoint.x().getVal(m_unit), i_physValPoint.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::setTopRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_rect.setTopRight(i_pt);
}

//------------------------------------------------------------------------------
void CPhysValRect::setTopRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_rect.setTopRight(QPointF(i_physValPoint.x().getVal(m_unit), i_physValPoint.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottomLeft(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_rect.setBottomLeft(i_pt);
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottomLeft(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_rect.setBottomLeft(QPointF(i_physValPoint.x().getVal(m_unit), i_physValPoint.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottomRight(const QPointF& i_pt)
//------------------------------------------------------------------------------
{
    m_rect.setBottomRight(i_pt);
}

//------------------------------------------------------------------------------
void CPhysValRect::setBottomRight(const CPhysValPoint& i_physValPoint)
//------------------------------------------------------------------------------
{
    m_rect.setBottomRight(QPointF(i_physValPoint.x().getVal(m_unit), i_physValPoint.y().getVal(m_unit)));
}

//------------------------------------------------------------------------------
void CPhysValRect::setResolution( double i_fRes )
//------------------------------------------------------------------------------
{
    m_fRes = i_fRes;
}

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
/*! @brief Returns the physical line as a QRectF instance.
*/
QRectF CPhysValRect::toQRectF() const
//------------------------------------------------------------------------------
{
    return m_rect;
}

//------------------------------------------------------------------------------
QString CPhysValRect::toString() const
//------------------------------------------------------------------------------
{
    return left().toString() + ", " + top().toString() + ", " + width().toString() + ", " + height().toString();
}
