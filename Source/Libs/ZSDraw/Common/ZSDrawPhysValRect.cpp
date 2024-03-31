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
/*! @brief Creates a physical rectangle on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_ptTL, i_ptBR),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QPointF& i_ptTL, const QPointF& i_ptBR, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_ptTL, i_ptBR),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_rect),
    m_unit(i_drawingScene.drawingSize().unit())
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CDrawingScene& i_drawingScene, const QRectF& i_rect, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_rect),
    m_unit(i_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValPoint& i_physValBottomRight) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_physValTopLeft.toQPointF(), i_physValBottomRight.toQPointF()),
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
CPhysValRect::CPhysValRect(
    const CDrawingScene& i_drawingScene,
    const CPhysValPoint& i_physValTopLeft,
    const CPhysValSize& i_physValSize) :
//------------------------------------------------------------------------------
    m_pDrawingScene(&i_drawingScene),
    m_rect(i_physValTopLeft.toQPointF(), i_physValSize.toQSizeF()),
    m_unit(i_physValTopLeft.unit())
{
    if (i_physValTopLeft.unit() != i_physValSize.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValTopLeft.resolution() != i_physValSize.resolution()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValRect::CPhysValRect(const CPhysValRect& i_physValRectOther) :
//------------------------------------------------------------------------------
    m_pDrawingScene(i_physValRectOther.m_pDrawingScene),
    m_rect(i_physValRectOther.m_rect),
    m_unit(i_physValRectOther.m_unit)
{
}

//------------------------------------------------------------------------------
CPhysValRect::~CPhysValRect()
//------------------------------------------------------------------------------
{
    m_pDrawingScene = nullptr;
    //m_rect;
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
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.top(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::bottom() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.bottom(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::left() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.left(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::right() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.right(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::width() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.width(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValRect::height() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_rect.height(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysValSize CPhysValRect::size() const
//------------------------------------------------------------------------------
{
    return CPhysValSize(*m_pDrawingScene, m_rect.size(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::center() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_rect.center(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_rect.left(), m_rect.top(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::topRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_rect.right(), m_rect.top(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomLeft() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_rect.left(), m_rect.bottom(), m_unit);
}

//------------------------------------------------------------------------------
CPhysValPoint CPhysValRect::bottomRight() const
//------------------------------------------------------------------------------
{
    return CPhysValPoint(*m_pDrawingScene, m_rect.right(), m_rect.bottom(), m_unit);
}

//------------------------------------------------------------------------------
double CPhysValRect::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
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
QString CPhysValRect::toString(bool i_bAddUnit, const QString& i_strSeparator) const
//------------------------------------------------------------------------------
{
    QString str = left().toString(EUnitFind::None, PhysValSubStr::Val)
                + i_strSeparator
                + top().toString(EUnitFind::None, PhysValSubStr::Val)
                + i_strSeparator
                + width().toString(EUnitFind::None, PhysValSubStr::Val)
                + i_strSeparator
                + height().toString(EUnitFind::None, PhysValSubStr::Val);
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    return str;
}
