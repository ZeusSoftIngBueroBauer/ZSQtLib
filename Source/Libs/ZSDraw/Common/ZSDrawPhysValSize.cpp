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
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSPhysVal/ZSPhysValExceptions.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Draw;


/*******************************************************************************
class CPhysValSize
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates a physical size on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValSize::CPhysValSize(const CDrawingScene& i_drawingScene) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_size()
{
}

//------------------------------------------------------------------------------
/*! @brief Creates a physical size on the drawing scene in the current unit
           and current resolution of the drawing scene.
*/
CPhysValSize::CPhysValSize(const CDrawingScene& i_drawingScene, const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_size()
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(
    const CDrawingScene& i_drawingScene,
    double i_fWidth, double i_fHeight) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_size(i_fWidth, i_fHeight)
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(
    const CDrawingScene& i_drawingScene,
    double i_fWidth, double i_fHeight,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_size(i_fWidth, i_fHeight)
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(
    const CDrawingScene& i_drawingScene,
    const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_physValWidth.unit()),
    m_size(i_physValWidth.getVal(), i_physValHeight.getVal())
{
    if (i_physValWidth.unit() != i_physValHeight.unit()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
    if (i_physValWidth.getRes() != i_physValHeight.getRes()) {
        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
    }
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(
    const CDrawingScene& i_drawingScene,
    const QSizeF& i_size) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene),
    m_size(i_size)
{
}

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(
    const CDrawingScene& i_drawingScene,
    const QSizeF& i_size,
    const CUnit& i_unit) :
//------------------------------------------------------------------------------
    CPhysValShape(i_drawingScene, i_unit),
    m_size(i_size)
{
}

///*==============================================================================
//public: // ctors
//==============================================================================*/
//
////------------------------------------------------------------------------------
///*! @brief Creates a physical size on the drawing scene in the current unit
//           and current resolution of the drawing scene.
//*/
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_size()
//{
//}
//
////------------------------------------------------------------------------------
///*! @brief Creates a physical size on the drawing scene in the current unit
//           and current resolution of the drawing scene.
//*/
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_size()
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    double i_fWidth, double i_fHeight) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_size(i_fWidth, i_fHeight)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    double i_fWidth, double i_fHeight,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_size(i_fWidth, i_fHeight)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_physValWidth.unit()),
//    m_size(i_physValWidth.getVal(), i_physValHeight.getVal())
//{
//    if (i_physValWidth.unit() != i_physValHeight.unit()) {
//        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
//    }
//    if (i_physValWidth.getRes() != i_physValHeight.getRes()) {
//        throw CException(__FILE__, __LINE__, EResultArgOutOfRange);
//    }
//}
//
////------------------------------------------------------------------------------
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QSizeF& i_size) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY),
//    m_size(i_size)
//{
//}
//
////------------------------------------------------------------------------------
//CPhysValSize::CPhysValSize(
//    const CDrawingScene& i_drawingScene,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsX,
//    const GUI::Math::CScaleDivLinesMetrics& i_divLinesMetricsY,
//    const QSizeF& i_size,
//    const CUnit& i_unit) :
////------------------------------------------------------------------------------
//    CPhysValShape(i_drawingScene, i_divLinesMetricsX, i_divLinesMetricsY, i_unit),
//    m_size(i_size)
//{
//}

/*==============================================================================
public: // copy ctor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize::CPhysValSize(const CPhysValSize& i_physValSizeOther) :
//------------------------------------------------------------------------------
    CPhysValShape(i_physValSizeOther),
    m_size(i_physValSizeOther.m_size)
{
}

/*==============================================================================
public: // dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize::~CPhysValSize()
//------------------------------------------------------------------------------
{
    //m_size;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValSize& CPhysValSize::operator = ( const QSizeF& i_size )
//------------------------------------------------------------------------------
{
    m_size = i_size;
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
CPhysValSize& CPhysValSize::operator = ( const QString& i_strValOther )
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
    CPhysVal physValWidth(m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
    CPhysVal physValHeight(m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
    physValWidth = strlst[0];
    physValHeight = strlst[1];
    m_unit = physValWidth.unit();
    physValHeight.convertValue(m_unit);
    m_size.setWidth(physValWidth.getVal());
    m_size.setHeight(physValHeight.getVal());
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
CPhysValShape& CPhysValSize::operator = ( const CPhysValShape& i_physValSizeOther )
//------------------------------------------------------------------------------
{
    const CPhysValSize& physValSizeOther = dynamic_cast<const CPhysValSize&>(i_physValSizeOther);
    //m_pDivLinesMetricsX = physValSizeOther.m_pDivLinesMetricsX;
    //m_pDivLinesMetricsY = physValSizeOther.m_pDivLinesMetricsY;
    m_unit = physValSizeOther.m_unit;
    m_size = physValSizeOther.m_size;
    m_unit = physValSizeOther.m_unit;
    return *this;
}

/*==============================================================================
public: // must overridable operators of base class CPhysValShape
==============================================================================*/

//------------------------------------------------------------------------------
bool CPhysValSize::operator == ( const CPhysValShape& i_physValSizeOther ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    const CPhysValSize& physValSizeOther = dynamic_cast<const CPhysValSize&>(i_physValSizeOther);
    if (!areOfSameUnitGroup(m_unit, physValSizeOther.m_unit)) {
        bEqual = false;
    }
    else if (m_unit == physValSizeOther.m_unit && m_size != physValSizeOther.m_size) {
        bEqual = false;
    }
    else if (m_unit != physValSizeOther.m_unit && m_size == physValSizeOther.m_size) {
        bEqual = false;
    }
    else if (m_unit != physValSizeOther.m_unit && m_size != physValSizeOther.m_size) {
        double fWidth = physValSizeOther.width().getVal(m_unit);
        double fHeight = physValSizeOther.height().getVal(m_unit);
        if (fWidth != m_size.width() || fHeight != m_size.height()) {
            bEqual = false;
        }
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CPhysValSize::operator != ( const CPhysValShape& i_physValSizeOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_physValSizeOther);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Invalidates the size by setting width and height to 0.0.
*/
void CPhysValSize::invalidate()
//------------------------------------------------------------------------------
{
    m_size = QSizeF(0.0, 0.0);
}

//------------------------------------------------------------------------------
bool CPhysValSize::isValid() const
//------------------------------------------------------------------------------
{
    return m_size.isValid();
}

//------------------------------------------------------------------------------
/*! @brief Returns true if the rectangle is null, otherwise returns false.

    A null rectangle has both width and height equal to 0.0.
*/
bool CPhysValSize::isNull() const
//------------------------------------------------------------------------------
{
    return m_size.isNull();
}

//------------------------------------------------------------------------------
/*! @brief Draws the shape on the drawing scene.
*/
void CPhysValSize::draw(QPainter* i_pPainter, const QRectF& i_rect, const CDrawSettings& i_drawSettings)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
QString CPhysValSize::toString(bool i_bAddUnit, const QString& i_strSeparator) const
//------------------------------------------------------------------------------
{
    QString str = width().toString(EUnitFind::None, PhysValSubStr::Val)
                + i_strSeparator
                + height().toString(EUnitFind::None, PhysValSubStr::Val);
    if (i_bAddUnit) {
        str += " " + m_unit.symbol();
    }
    return str;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CPhysVal CPhysValSize::width() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_size.width(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
CPhysVal CPhysValSize::height() const
//------------------------------------------------------------------------------
{
    const CDrawingSize& drawingSize = m_pDrawingScene->drawingSize();
    return CPhysVal(m_size.height(), m_unit, drawingSize.imageCoorsResolution(m_unit).getVal());
}

//------------------------------------------------------------------------------
double CPhysValSize::resolution() const
//------------------------------------------------------------------------------
{
    return m_pDrawingScene->drawingSize().imageCoorsResolution(m_unit).getVal();
}

//------------------------------------------------------------------------------
CUnit CPhysValSize::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

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
/*! @brief Returns the physical point as a QPointF instance in the desired unit.
* 
*/
QSizeF CPhysValSize::toQSizeF(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.unitsAreEitherMetricOrNot(i_unit, m_unit)) {
        throw CUnitConversionException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    QSizeF sizeF = m_size;
    if (i_unit != m_unit) {
        sizeF.setWidth(CPhysVal(m_size.width(), m_unit).getVal(i_unit));
        sizeF.setHeight(CPhysVal(m_size.height(), m_unit).getVal(i_unit));
    }
    return sizeF;
}
