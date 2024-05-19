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
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValPoint
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical point on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPoint::CPhysValPoint(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_pt()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical point on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPoint::CPhysValPoint(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
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
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    double i_fX, double i_fY) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_pt(i_fX, i_fY)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    double i_fX, double i_fY,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_pt(i_fX, i_fY)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_pt) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_pt(i_pt)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const QPointF& i_pt,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_pt(i_pt)
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_physValX.unit()),
    m_pt(i_physValX.getVal(), i_physValY.getVal())
{
    if (i_physValX.unit() != i_physValY.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValX.getRes() != i_physValY.getRes()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical point on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
    m_pt()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical point on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    const CUnit& i_unit ) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
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
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    double i_fX, double i_fY) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
    m_pt(i_fX, i_fY)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    double i_fX, double i_fY,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
    m_pt(i_fX, i_fY)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    const QPointF& i_pt) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
    m_pt(i_pt)
{
}

//------------------------------------------------------------------------------
/*! @brief

    @param [in] i_pt
        Point whose coordinates are in the given unit.
    @param [in] i_unit
        Unit the coordinates are passed.
*/
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    const QPointF& i_pt,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
    m_pt(i_pt)
{
}

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint(
    const CDrawingScene& i_drawingScene,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
    const CPhysVal& i_physValX, const CPhysVal& i_physValY) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_physValX.unit()),
    m_pt(i_physValX.getVal(), i_physValY.getVal())
{
    if (i_physValX.unit() != i_physValY.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValX.getRes() != i_physValY.getRes()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint::CPhysValPoint( const CPhysValPoint& i_physValPointOther ) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValPointOther),
    m_pt(i_physValPointOther.m_pt)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint::~CPhysValPoint()
//------------------------------------------------------------------------------
{
    //m_pt;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValPoint& CPhysValPoint::operator = ( const QPointF& i_ptOther )
//------------------------------------------------------------------------------
{
    m_pt = i_ptOther;
    return *this;
}

//------------------------------------------------------------------------------
/*! @brief Assigns the string containing a physical point definition to this.

    The current resolution remains unchanged.
    If no unit is defined in the input string
        - the current unit remains unchanged or is
        - is set to "px" if no current unit has been assigned yet.

    @param [in] i_strValOther
        String containing the physical point definition in the following format:
        "X [unit] / Y [unit]".

    @return New physical point value.

    @note throws a CUnitConversionException if conversion fails.
*/
CPhysValPoint& CPhysValPoint::operator = ( const QString& i_strValOther )
//------------------------------------------------------------------------------
{
    QStringList strlst = i_strValOther.split("/");
    if (strlst.size() != 2) {
        strlst = i_strValOther.split(",");
    }
    if (strlst.size() != 2) {
        throw CUnitConversionException(
            __FILE__, __LINE__, EResultArgOutOfRange, i_strValOther);
    }
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    CPhysVal physValX(m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
    CPhysVal physValY(m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
    physValX = strlst[0];
    physValY = strlst[1];
    m_unit = physValX.unit();
    physValY.convertValue(m_unit);
    m_pt.setX(physValX.getVal());
    m_pt.setY(physValY.getVal());
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValPoint::operator = ( const CPhysValShape& i_physValPointOther )
//------------------------------------------------------------------------------
{
    const CPhysValPoint& physValPointOther = dynamic_cast<const CPhysValPoint&>(i_physValPointOther);
    m_pDivLinesMetricsX = physValPointOther.m_pDivLinesMetricsX;
    m_pDivLinesMetricsY = physValPointOther.m_pDivLinesMetricsY;
    m_unit = physValPointOther.m_unit;
    m_pt = physValPointOther.m_pt;
    m_unit = physValPointOther.m_unit;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValPoint::operator == ( const CPhysValShape& i_physValPointOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    const CPhysValPoint& physValPointOther = dynamic_cast<const CPhysValPoint&>(i_physValPointOther);
    if (!areOfSameUnitGroup(m_unit, physValPointOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit == physValPointOther.m_unit && m_pt != physValPointOther.m_pt) {
        bEqual = false;
    }
    else if (m_unit != physValPointOther.m_unit && m_pt == physValPointOther.m_pt) {
        bEqual = false;
    }
    else if (m_unit != physValPointOther.m_unit && m_pt != physValPointOther.m_pt) {
        double fXOther = physValPointOther.x().getVal(m_unit);
        double fYOther = physValPointOther.y().getVal(m_unit);
        if (fXOther != m_pt.x() || fYOther != m_pt.y()) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValPoint::operator != ( const CPhysValShape& i_physValPointOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValPointOther);
}

/*==============================================================================
public: // must overridables of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is valid, otherwise returns false.

    A valid rectangle has both width and height greater than 0.0.
*/
bool CPhysValPoint::isValid() const
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is null, otherwise returns false.

    A null rectangle has both width and height equal to 0.0.
*/
bool CPhysValPoint::isNull() const
//------------------------------------------------------------------------------
{
    return false;
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValPoint::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
QString CPhysValPoint::toString(bool i_bAddUnit, const QString& i_strSeparator) const
//------------------------------------------------------------------------------
{
    QString str = x().toString(EUnitFind::None, PhysValSubStr::Val)
                + i_strSeparator
                + y().toString(EUnitFind::None, PhysValSubStr::Val);
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    return str;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CPhysValPoint::x() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_pt.x(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValPoint::y() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_pt.y(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
double CPhysValPoint::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
}

//------------------------------------------------------------------------------
CUnit CPhysValPoint::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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
/*! @brief Returns the physical point as a QPointF instance in the current unit.
*/
QPointF CPhysValPoint::toQPointF() const
//------------------------------------------------------------------------------
{
    return m_pt;
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical point as a QPointF instance in the desired unit.
* 
*/
QPointF CPhysValPoint::toQPointF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QPointF pt = m_pt;
    if (i_unit != m_unit) {
        pt.setX(CPhysVal(m_pt.x(), m_unit).getVal(i_unit));
        pt.setY(CPhysVal(m_pt.y(), m_unit).getVal(i_unit));
    }
    return pt;
}
