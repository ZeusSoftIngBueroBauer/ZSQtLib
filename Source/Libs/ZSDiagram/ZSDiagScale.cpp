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

#include "ZSDiagram/ZSDiagScale.h"
#include "ZSDiagram/ZSDiagramProcData.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Diagram;
using namespace ZS::PhysVal;


/*******************************************************************************
class CScale
*******************************************************************************/

/*==============================================================================
public: // ctors
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Default constructor creating a scale instance with invalid values.
*/
CScale::CScale() :
//------------------------------------------------------------------------------
    m_fMin(0.0),
    m_fMax(0.0),
    m_unit(),
    m_physValRes()
{
} // default ctor

//------------------------------------------------------------------------------
/*! @brief Copy constructor.

    @param i_scaleOther [in]
        Scale object to be copied.
*/
CScale::CScale( const CScale& i_scaleOther ) :
//------------------------------------------------------------------------------
    m_fMin(i_scaleOther.m_fMin),
    m_fMax(i_scaleOther.m_fMax),
    m_unit(i_scaleOther.m_unit),
    m_physValRes(i_scaleOther.m_physValRes)
{
} // copy ctor

//------------------------------------------------------------------------------
/*! @brief Creates a scale instance.

    @param i_fMin [in]
        Minimum scale value.
    @param i_fMax [in]
        Maximum scale value.
    @param i_unit [in]
        Unit of minimum and maximum scale value.
    @param i_physValRes [in]
        Resolution of the minimum and maximum scale values.
*/
CScale::CScale(double i_fMin, double i_fMax, const CUnit& i_unit, const CPhysValRes& i_physValRes) :
//------------------------------------------------------------------------------
    m_fMin(i_fMin),
    m_fMax(i_fMax),
    m_unit(i_unit),
    m_physValRes(i_physValRes)
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the scale's minimum value.

    The given value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_physVal [in]
        Minimum value.
*/
void CScale::setMinVal(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    m_fMin = i_physVal.getVal(m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's minimum value.

    The given value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_fVal [in]
        Minimum value in the unit as passed in argument i_unit.
    @param i_unit [in]
        Unit of the given value.
*/
void CScale::setMinVal(double i_fVal, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    if (i_unit == m_unit) {
        m_fMin = i_fVal;
    }
    else {
        m_fMin = i_unit.convertValue(i_fVal, m_unit);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum value of the scale including the scale's resolution.

    @return Minimum value.
*/
CPhysVal CScale::minVal() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fMin, m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's maximum value.

    The given value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_physVal [in]
        Maximum value.
*/
void CScale::setMaxVal(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    m_fMax = i_physVal.getVal(m_unit);
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's maximum value.

    The given value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_fVal [in]
        Maximum value in the unit as passed in argument i_unit.
    @param i_unit [in]
        Unit of the given value.
*/
void CScale::setMaxVal(double i_fVal, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    if (i_unit == m_unit) {
        m_fMax = i_fVal;
    }
    else {
        m_fMax = i_unit.convertValue(i_fVal, m_unit);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum value of the scale including the scale's resolution.

    @return Maximum value.
*/
CPhysVal CScale::maxVal() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fMax, m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's range by adjusting the maximum value.

    The minimum value remains the same.

    The given range value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_physVal [in]
        Range value.
*/
void CScale::setRangeVal(const CPhysVal& i_physVal)
//------------------------------------------------------------------------------
{
    double fValRange = i_physVal.getVal(m_unit);
    m_fMax = m_fMin + fValRange;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's range by adjusting the maximum value.

    The minimum value remains the same.

    The given range value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_fVal [in]
        Range value in the unit as passed in argument i_unit.
    @param i_unit [in]
        Unit of the given value.
*/
void CScale::setRangeVal(double i_fVal, const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    double fValRange = i_unit.convertValue(i_fVal, m_unit);
    m_fMax = m_fMin + fValRange;
}

//------------------------------------------------------------------------------
/*! @brief Returns the range of the scale including the scale's resolution.

    @return Range value.
*/
CPhysVal CScale::rangeVal() const
//------------------------------------------------------------------------------
{
    double fRange = fabs(m_fMax - m_fMin);
    return CPhysVal(fRange, m_unit, m_physValRes);
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale unit.

    The minimum and maximum values are converted into the new unit.
    Also the resolution will be converted into the new unit.
    If the values cannot be converted a CUnitConversionException is thrown.

    @param i_unit [in]
        Unit to be set.
*/
void CScale::setUnit(const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    m_fMin = m_unit.convertValue(m_fMin, i_unit);
    m_fMax = m_unit.convertValue(m_fMax, i_unit);
    m_unit = i_unit;
    m_physValRes.convertValue(i_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the unit of the scale.

    @return Unit of the scale.
*/
CUnit CScale::unit() const
//------------------------------------------------------------------------------
{
    return m_unit;
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's resolution.

    The given resolution will be converted into the scales unit.
    If the value cannot be converted a CUnitConversionException is thrown.

    @param i_physValRes [in]
        Resolution of the scale values.
*/
void CScale::setRes(const CPhysValRes& i_physValRes)
//------------------------------------------------------------------------------
{
    if (i_physValRes.unit() != m_unit) {
        CPhysValRes physValRes = i_physValRes;
        physValRes.convertValue(m_unit);
        m_physValRes = physValRes; // only set if value is convertible
    }
    else {
        m_physValRes = i_physValRes;
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the range of the scale including the scale's resolution.

    @return Range value.
*/
CPhysValRes CScale::res() const
//------------------------------------------------------------------------------
{
    return m_physValRes;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Equal to operator comparing this value with given second operand.

    @param i_scaleOther [in]
        Second operator.

    @return true, if this is equal to the second operand, false otherwise.
*/
bool CScale::operator == ( const CScale& i_scaleOther ) const
//------------------------------------------------------------------------------
{
    //lint -e777
    bool bEqual = false;
    if( m_unit == i_scaleOther.m_unit
     && m_fMin == i_scaleOther.m_fMin
     && m_fMax == i_scaleOther.m_fMax
     && m_physValRes == i_scaleOther.m_physValRes )
    {
        bEqual = true;
    }
    return bEqual;
    //lint +e777
}

//------------------------------------------------------------------------------
/*! @brief Not equal to operator comparing this value with given second operand.

    @param i_scaleOther [in]
        Second operator.

    @return true, if this is not equal to the second operand, false otherwise.
*/
bool CScale::operator != ( const CScale& i_scaleOther ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_scaleOther); //lint !e1702
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Converts the scale to a string representation.

    Mainly used for debugging and logging purposes.

    @return String representation of the scale.
*/
QString CScale::toString() const
//------------------------------------------------------------------------------
{
    QString str =
        "Min: " + QString::number(m_fMin) +
        ", Max: " + QString::number(m_fMax) +
        ", Unit: " + m_unit.symbol() +
        ", Res: " + m_physValRes.toString();
    return str;
}


/*******************************************************************************
class CDiagScale
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Creates an instance of the class.

    @param i_strObjName [in]
        Name of the object.
        Each scale object belonging to the same diagram must have a unique name.
    @param i_scaleDir [in]
        Direction of the scale which could be either X or Y.
        3 dimensional scales with scaleDir = Z are not supported.
        Please note that the scale direction cannot be changed during runtime.
    @param i_scale [in]
        Minimum, maximum values and unit of the scale.
        The scale also contains the resolution of the scale which defines the
        number of significant digits when converting the minimum and maximum
        value to their string representations.
*/
CDiagScale::CDiagScale(
    const QString& i_strObjName,
    EScaleDir      i_scaleDir,
    const CScale&  i_scale ) :
//------------------------------------------------------------------------------
    QObject(),
    m_strObjName(i_strObjName),
    m_pDiagram(nullptr),
    m_scale(i_scale),
    m_divLines("DiagScale"+ i_strObjName, i_scaleDir),
    m_iZoomCount(0),
    m_pZoomStackFirst(nullptr),
    m_pZoomStackLast(nullptr),
    m_pTrcAdminObj(nullptr),
    m_pTrcAdminObjUpdate(nullptr),
    m_pTrcAdminObjLayout(nullptr)
{
    setObjectName(i_strObjName);

    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName(), objectName());
    m_pTrcAdminObjUpdate = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Update", objectName());
    m_pTrcAdminObjLayout = CTrcServer::GetTraceAdminObj(
        NameSpace(), ClassName() + "::Layout", objectName());

    QString strMthInArgs;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal))
    {
        strMthInArgs = i_strObjName;
        strMthInArgs += ", ScaleDir: " + CEnumScaleDir(i_scaleDir).toString();
        strMthInArgs += ", Scale: " + i_scale.toString();
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

    m_divLines.setScale(
        m_scale.minVal().getVal(), m_scale.maxVal().getVal(),
        m_scale.res().getVal(), 0, 0);
    m_divLines.setDivLinesDistMinInPix(EDivLineLayer::Main, 50);

} // ctor

//------------------------------------------------------------------------------
/*! @brief Destructor.
*/
CDiagScale::~CDiagScale()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    if( m_iZoomCount > 0 )
    {
        try
        {
            clearScaleStack();
        }
        catch(...)
        {
        }
    }

    //m_strObjName;
    m_pDiagram = nullptr;
    //m_scale;
    //m_divLines;
    m_iZoomCount = 0;
    m_pZoomStackFirst = nullptr;
    m_pZoomStackLast = nullptr;

    mthTracer.onAdminObjAboutToBeReleased();

    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);
    m_pTrcAdminObj = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjUpdate);
    m_pTrcAdminObjUpdate = nullptr;
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObjLayout);
    m_pTrcAdminObjLayout = nullptr;

} // dtor

/*==============================================================================
public: // instance methods (common properties)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the name of the object.

    @return Name of the object.
*/
QString CDiagScale::getObjName() const
//------------------------------------------------------------------------------
{
    return m_strObjName;
}

//------------------------------------------------------------------------------
/*! @brief Returns the direction of the scale (X or Y).

    @return Scale direction.
*/
EScaleDir CDiagScale::getScaleDir() const
//------------------------------------------------------------------------------
{
    return m_divLines.scaleDir();
}

//------------------------------------------------------------------------------
/*! @brief Returns the diagram the scale object belongs to.

    @return Diagram the scale belongs to.
*/
CDataDiagram* CDiagScale::getDiagram()
//------------------------------------------------------------------------------
{
    return m_pDiagram;
}

//------------------------------------------------------------------------------
/*! @brief Sets the spacing of the scale.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_spacing [in]
        Range [Linear, Logarithmic]
*/
void CDiagScale::setSpacing( const CEnumSpacing& i_spacing )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_spacing.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setSpacing",
        /* strAddInfo   */ strMthInArgs );

    if (m_divLines.setSpacing(i_spacing))
    {
        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_spacingChanged(m_divLines.spacing());
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the spacing of the scale which could be either Linear or Logarithmic.

    @return Spacing of the scale.
*/
ESpacing CDiagScale::getSpacing() const
//------------------------------------------------------------------------------
{
    return m_divLines.spacing();
}

/*==============================================================================
public: // instance methods (scale values, unit and resolution)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the scale is valid.

    A scale is valid if the geometries range in pixels is greater than 0 and the
    physical minimum value is lower than the maximum value.

    For logarithmic scales both the physical minimum and maximum values must be
    greater than 0.

    @return true, if the scale values are valid, false otherwise.
*/
bool CDiagScale::isScaleValid() const
//------------------------------------------------------------------------------
{
    bool bScaleValid = true;

    if( getRangePix() <= 0 )
    {
        bScaleValid = false;
    }
    else if( m_scale.maxVal() <= m_scale.minVal() )
    {
        bScaleValid = false;
    }
    else if( m_divLines.spacing() == ESpacing::Logarithmic )
    {
        if( m_scale.minVal().getVal() <= 0.0 || m_scale.maxVal().getVal() <= 0.0 )
        {
            bScaleValid = false;
        }
    }
    return bScaleValid;
}

//------------------------------------------------------------------------------
/*! @brief Sets the world coordinates (phyiscal values) of the scale object.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_scale [in]
        Scaling including the physical minimum and maximum values and their unit.
        The scaling also includes the resolution of the scale (number of significant
        digits to indicate the minimum and maximum values).
*/
void CDiagScale::setScale( const CScale& i_scale )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScale",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale != i_scale) //lint !e1702
    {
        m_scale = i_scale;

        m_divLines.setScaleMin(m_scale.minVal().getVal());
        m_divLines.setScaleMax(m_scale.maxVal().getVal());
        m_divLines.setScaleRes(m_scale.res().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the physical values (world coordinates) of the scale.

    @return Physical values of the scale.
*/
CScale CDiagScale::getScale() const
//------------------------------------------------------------------------------
{
    return m_scale;
}

//------------------------------------------------------------------------------
/*! @brief Sets the minimum value of the scale in world coordinates (phyiscal value).

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_physVal [in]
        Minimum scale value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
*/
void CDiagScale::setScaleMinVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleMinVal",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale.minVal() != i_physVal)
    {
        m_scale.setMinVal(i_physVal);

        m_divLines.setScaleMin(m_scale.minVal().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the minimum value of the scale in world coordinates (phyiscal value).

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_fVal [in]
        Minimum scale value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
    @param i_unit [in]
        Unit of the value in world coordinates.
*/
void CDiagScale::setScaleMinVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = QString::number(i_fVal) + " " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleMinVal",
        /* strAddInfo   */ strMthInArgs );

    setScaleMinVal(CPhysVal(i_fVal, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum scale value (world coordinates).

    @return Minimum scale value.
*/
CPhysVal CDiagScale::getScaleMinVal() const
//------------------------------------------------------------------------------
{
    return m_scale.minVal();
}

//------------------------------------------------------------------------------
/*! @brief Sets the maximum value of the scale in world coordinates (phyiscal value).

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_physVal [in]
        Maximum scale value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
*/
void CDiagScale::setScaleMaxVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleMaxVal",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale.maxVal() != i_physVal)
    {
        m_scale.setMaxVal(i_physVal);

        m_divLines.setScaleMax(m_scale.maxVal().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the minimum value of the scale in world coordinates (phyiscal value).

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_fVal [in]
        Minimum scale value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
    @param i_unit [in]
        Unit of the value in world coordinates.
*/
void CDiagScale::setScaleMaxVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = QString::number(i_fVal) + " " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleMaxVal",
        /* strAddInfo   */ strMthInArgs );

    setScaleMaxVal(CPhysVal(i_fVal, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum scale value (world coordinates).

    @return Maximum scale value.
*/
CPhysVal CDiagScale::getScaleMaxVal() const
//------------------------------------------------------------------------------
{
    return m_scale.maxVal();
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's range in world coordinates (phyiscal value) by
           adjusting the maximum value.

    The minimum value remains the same.

    The given range value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_physVal [in]
        Range value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
*/
void CDiagScale::setScaleRangeVal( const CPhysVal& i_physVal )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_physVal.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleRangeVal",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale.rangeVal() != i_physVal)
    {
        m_scale.setRangeVal(i_physVal);

        m_divLines.setScaleMax(m_scale.maxVal().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's range in world coordinates (phyiscal value) by
           adjusting the maximum value.

    The minimum value remains the same.

    The given range value will be converted into the unit of the scale.
    If the value cannot be converted a CUnitConversionException is thrown.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_fVal [in]
        Range value in world coordinates.
        The value will be converted into the scales unit.
        If the conversion fails the exception CUnitConversionException is thrown.
    @param i_unit [in]
        Unit of the value in world coordinates.
*/
void CDiagScale::setScaleRangeVal( double i_fVal, const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = QString::number(i_fVal) + " " + i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleRangeVal",
        /* strAddInfo   */ strMthInArgs );

    setScaleRangeVal(CPhysVal(i_fVal, i_unit));
}

//------------------------------------------------------------------------------
/*! @brief Returns the range of the scale (world coordinates).

    @return Range of scale (maxVal - minVal).
*/
CPhysVal CDiagScale::getScaleRangeVal() const
//------------------------------------------------------------------------------
{
    return m_scale.rangeVal();
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale unit.

    The minimum and maximum values are converted into the new unit.
    Also the resolution will be converted into the new unit.
    If the values cannot be converted a CUnitConversionException is thrown.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_unit [in]
        Unit to be set.
*/
void CDiagScale::setScaleUnit(const CUnit& i_unit)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_unit.symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleUnit",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale.unit() != i_unit)
    {
        m_scale.setUnit(i_unit);

        m_divLines.setScaleMin(m_scale.minVal().getVal());
        m_divLines.setScaleMax(m_scale.maxVal().getVal());
        m_divLines.setScaleRes(m_scale.res().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the unit of the scale.

    @return Unit of the scale.
*/
CUnit CDiagScale::getScaleUnit() const
//------------------------------------------------------------------------------
{
    return m_scale.unit();
}

//------------------------------------------------------------------------------
/*! @brief Sets the scale's resolution.

    The given resolution will be converted into the scales unit.
    If the value cannot be converted a CUnitConversionException is thrown.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_physValRes [in]
        Resolution of the minimum and maximum scale value.

    @return Resolution.
*/
void CDiagScale::setScaleRes( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave)) {
        strMthInArgs = i_physValRes.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleRes",
        /* strAddInfo   */ strMthInArgs );

    if (m_scale.res() != i_physValRes)
    {
        m_scale.setRes(i_physValRes);

        m_divLines.setScaleRes(m_scale.res().getVal());

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_scaleChanged(m_scale);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of the scale.

    @return Resolution.
*/
CPhysValRes CDiagScale::getScaleRes() const
//------------------------------------------------------------------------------
{
    return m_scale.res();
}

/*==============================================================================
public: // instance methods (geometry in pixels)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the geometry of the scale (pixel range).

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_iMinVal_px [in]
        Minimum value in pixels.
    @param i_iMaxVal_px [in]
        Maximum value in pixels.
*/
void CDiagScale::setGeometry( int i_iMinVal_px, int i_iMaxVal_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObjLayout, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iMinVal_px) + ", " + QString::number(i_iMaxVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjLayout,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "setGeometry",
        /* strAddInfo   */ strMthInArgs );

    if ((m_divLines.scaleMinInPix() != i_iMinVal_px) || (m_divLines.scaleMaxInPix() != i_iMaxVal_px))
    {
        m_divLines.setScaleMinInPix(i_iMinVal_px);
        m_divLines.setScaleMaxInPix(i_iMaxVal_px);

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_geometryChanged(m_divLines.scaleMaxInPix(), m_divLines.scaleMaxInPix());
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum geometry value in pixels.

    @Return Minimum value in pixels.
*/
int CDiagScale::getMinValPix() const
//------------------------------------------------------------------------------
{
    return m_divLines.scaleMinInPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum geometry value in pixels.

    @Return Maximum value in pixels.
*/
int CDiagScale::getMaxValPix() const
//------------------------------------------------------------------------------
{
    return m_divLines.scaleMaxInPix();
}

//------------------------------------------------------------------------------
/*! @brief Returns the range in pixels.

    @Return Range in pixels.
*/
int CDiagScale::getRangePix() const
//------------------------------------------------------------------------------
{
    return m_divLines.scaleRangeInPix();
}

/*==============================================================================
public: // instance methods (calculation of division lines)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the minimum distance between two division lines in pixels.

    The final distance between two division lines is calculated based on this value
    so that the division line values become a whole number of a decimal power.

    @note For the changed setting to take effect and become visible in the
          diagrams pixmap you must call the diagrams update method afterwards.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be applied.
    @param i_iDistMinPix [in]
        Minimum value in pixels.
*/
void CDiagScale::setDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer, int i_iDistMinPix )
//------------------------------------------------------------------------------
{
    QString strTrcMsg;

    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::EnterLeave))
    {
        strTrcMsg = "Layer: " + i_eLayer.toString();
        strTrcMsg += ", DistMinPix: " + QString::number(i_iDistMinPix);
    }

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDivLineDistMinPix",
        /* strAddInfo   */ strTrcMsg );

    if( m_divLines.setDivLinesDistMinInPix(i_eLayer, i_iDistMinPix))
    {
        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_divLineDistMinPixChanged(i_eLayer.enumerator(), i_iDistMinPix);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum distance between two division lines in pixels.

    The final distance between two division lines is calculated based on this value
    so that the division line values become a whole number of a decimal power.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be applied.

    @return Minimum distance between two division lines in pixels.
*/
int CDiagScale::getDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer ) const
//------------------------------------------------------------------------------
{
    return m_divLines.divLinesDistMinInPix(i_eLayer);
}

//------------------------------------------------------------------------------
/*! @brief Returns whether the division line values have been calculated.

    Usually there should be no need to call this method explicitly.
    The flag is internally set to false if properties are changed and
    set to true if the division line values have been recalculated by
    the update method.

    @return true, if calculated, false otherwise.
*/
bool CDiagScale::areDivLinesCalculated() const
//------------------------------------------------------------------------------
{
    return m_divLines.divLinesCalculated();
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of calculated division lines for the given layer.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.

    @return Number of division lines.
*/
int CDiagScale::getDivLineCount( const CEnumDivLineLayer& i_eLayer ) const
//------------------------------------------------------------------------------
{
    return m_divLines.getDivLinesCount(i_eLayer);
}

//------------------------------------------------------------------------------
/*! @brief Returns the calculated distance between two division lines for the
           given layer.

    For linear spaced scales the distance between two successive division lines
    is the same. For logarithmic scales the minimum value is returned.

    @param i_eLayer [in]
        Range [Main, Sub, Invalid]
        Layer for which the value should be returned.
        You may pass an invalid enum value if you want to consider all layers.
    @param i_pUnit [in]
        You may pass a pointer to a unit if you want to convert the value.

    @return Number of division lines.
*/
double CDiagScale::getDivLineDistMin(
    const CEnumDivLineLayer& i_eLayer, const PhysVal::CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();
    if( i_pUnit != nullptr ) {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) ) {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    double fDivLineDistMin = m_divLines.getDivLinesDistMin(i_eLayer);
    return m_scale.unit().convertValue(fDivLineDistMin, unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the value at the specified division line.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.
    @param i_pUnit [in]
        You may pass a pointer to a unit if you want to convert the value.

    @return Value in world coordinates of the division line.
*/
double CDiagScale::getDivLineVal(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();
    if( i_pUnit != nullptr ) {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) ) {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    double fDivLineVal = m_divLines.getDivLineVal(i_eLayer, i_idxDivLine);
    return m_scale.unit().convertValue(fDivLineVal, unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the value in pixels at the specified division line.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine [in]
        Range [0..DivLineCount-1]
        Index of the division line the value should be returned.

    @return Value in pixel coordinates of the division line.
*/
double CDiagScale::getDivLinePix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine ) const
//------------------------------------------------------------------------------
{
    return m_divLines.getDivLineInPix(i_eLayer, i_idxDivLine);
}

//------------------------------------------------------------------------------
/*! @brief Returns the distance in world coordinates (physical values)
           between the specified division lines.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine1 [in]
        Range [0..DivLineCount-1]
        Index of the first division line to be used.
    @param i_idxDivLine2 [in]
        Range [0..DivLineCount-1]
        Index of the second division line to be used.
    @param i_pUnit [in]
        You may pass a pointer to a unit if you want to convert the value.

    @return Distance in world coordinates between the two division lines.
*/
double CDiagScale::getDivLineDistVal(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2,
    const PhysVal::CUnit* i_pUnit) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();
    if( i_pUnit != nullptr ) {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) ) {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }
    double fDivLineDist = m_divLines.getDivLineDistVal(i_eLayer, i_idxDivLine1, i_idxDivLine2);
    return m_scale.unit().convertValue(fDivLineDist, unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the distance in pixels between the specified division lines.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be returned.
    @param i_idxDivLine1 [in]
        Range [0..DivLineCount-1]
        Index of the first division line to be used.
    @param i_idxDivLine2 [in]
        Range [0..DivLineCount-1]
        Index of the second division line to be used.

    @return Distance in pixel coordinates between the two division lines.
*/
double CDiagScale::getDivLineDistPix(
    const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2 ) const
//------------------------------------------------------------------------------
{
    return m_divLines.getDivLineDistInPix(i_eLayer, i_idxDivLine1, i_idxDivLine2);
}

/*==============================================================================
public: // instance mehods (calculation of division lines)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns a reference to the internal division lines scaler instance.

    The returned reference may not be changed by the caller as it contains
    the internal data of the diagram scale object. But the referen may be
    used to initialize scalers outside of the diagram scale object.
    For example by the diagram axis label object to assign the current
    division line calculation result for further data processing like
    calculating the extent of the division line labels.

    @return Reference to division line scaler.
*/
const ZS::System::Math::CScaleDivLines& CDiagScale::getDivLinesScaler() const
//------------------------------------------------------------------------------
{
    return m_divLines;
}

/*==============================================================================
public: // instance methods (converting values)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the value in pixels for the given value.

    @param i_fVal [in]
        Value in world coordinates (physical value)
    @param i_pUnit [in]
        You may pass a pointer to a unit if the passed value is different
        from the scale unit.

    @return Value in pixel coordinates of the given value.
*/
int CDiagScale::getValPix( double i_fVal, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();
    if( i_pUnit != nullptr ) {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) ) {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fPix = static_cast<double>(m_divLines.scaleMinInPix());

    if( getRangePix() > 1 && isScaleValid() )
    {
        double fVal = unit.convertValue(i_fVal, m_scale.unit());
        fPix = m_divLines.getValInPix(fVal);
    }
    return static_cast<int>(fPix);
}

//------------------------------------------------------------------------------
/*! @brief Returns the value in world coordinates (physical value) for the
           given position in pixels.

    @param i_fPix [in]
        Value in pixel coordinates.
    @param i_pUnit [in]
        You may pass a pointer to a unit if the returned value should be
        converted into another unit than the current scale unit.

    @return Value in world coordinates.
*/
double CDiagScale::getVal( double i_fPix, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();
    if( i_pUnit != nullptr ) {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) ) {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    double fVal = m_scale.minVal().getVal(unit);

    if( getRangePix() > 1 && isScaleValid() )
    {
        fVal = m_divLines.getVal(i_fPix);
        fVal = m_scale.unit().convertValue(fVal, unit);
    }
    return fVal;
}

//------------------------------------------------------------------------------
/*! @brief Returns the given value into its string representation.

    @param i_fVal [in]
        Value in world coordinates (physical value)
    @param i_pUnit [in]
        You may pass a pointer to a unit if the passed value is different
        from the scale unit.
    @param i_iDigitsCountMax [in]
        Maximum number of digits used for the output string.
    @param i_bUseEngineeringFormat [in]
        true if the value should be converted using engineering format,
        false otherwise.

    @return Value converted to string.
*/
QString CDiagScale::getValString(
    double i_fVal, const CUnit* i_pUnit,
    int i_iDigitsCountMax, bool i_bUseEngineeringFormat ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    QString strVal = "---";

    if( isScaleValid() )
    {
        // The scale resolution defines the number of valid digits.

        double fVal = i_fVal;
        int    iLeadingDigits = 1;
        int    iTrailingDigits = 1;
        int    iExponentDigits = 0;
        double fScaleRes = getScaleRes().getVal(unit);

        if( fVal >= 10.0 )
        {
            iLeadingDigits = static_cast<int>(log10(fabs(fVal)))+1;
        }
        if( fScaleRes < 0.1 )
        {
            double fScaleResLog = log10(fabs(fScaleRes));
            double fScaleResLogInt;
            double fScaleResLogFrac = modf(fScaleResLog,&fScaleResLogInt);
            iTrailingDigits = static_cast<int>(-fScaleResLog);
            if( fScaleResLogFrac > 0.0 )
            {
                iTrailingDigits += 1;
            }
        }
        if( i_iDigitsCountMax > 0 || i_bUseEngineeringFormat )
        {
            if( ( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
             || ( i_bUseEngineeringFormat && (iLeadingDigits > 1) ) )
            {
                if( fVal > 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iLeadingDigits)))+1;
                }
                else if( fVal < 1.0 )
                {
                    iExponentDigits = static_cast<int>(log10(static_cast<double>(iTrailingDigits)))+1;
                }
                iTrailingDigits += iLeadingDigits-1;
                iLeadingDigits = 1;
                if( (iLeadingDigits + iTrailingDigits) > i_iDigitsCountMax )
                {
                    iTrailingDigits = i_iDigitsCountMax - iLeadingDigits;
                }
            }
        }
        if( iExponentDigits > 0 )
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'e',
                /* iPrecision */ iTrailingDigits );
        }
        else
        {
            strVal = QString::number(
                /* fVal       */ fVal,
                /* chFormat   */ 'f',
                /* iPrecision */ iTrailingDigits );
        }
    }
    return strVal;

} // getValString

/*==============================================================================
public: // instance methods (zooming)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the scale to the new zoom geometry and pushes the current
           scale on the zoom stack.

    @param i_iZoomRectMinValPix [in]
        Minimum value in pixels to which the current scale should be zoomed in.
        The new scale minimum value will be set to the value at this given
        pixel coordinate.
    @param i_iZoomRectMaxValPix [in]
        Maximum value in pixels to which the current scale should be zoomed in.
        The new scale maximum value will be set to the value at this given
        pixel coordinate.
*/
void CDiagScale::zoomIn( int i_iZoomRectMinValPix, int i_iZoomRectMaxValPix )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iZoomRectMinValPix) + ", " + QString::number(i_iZoomRectMaxValPix);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomIn",
        /* strAddInfo   */ strMthInArgs );

    CScale scaleZoom = m_scale;
    scaleZoom.setMinVal(getVal(static_cast<double>(i_iZoomRectMinValPix)));
    scaleZoom.setMaxVal(getVal(static_cast<double>(i_iZoomRectMaxValPix)));
    pushScale(m_scale);
    setScale(scaleZoom); // sets the flag calculated to false and calls "scaleChanged" of diagram
}

//------------------------------------------------------------------------------
/*! @brief Sets new scale values and pushes the current scale onto the zoom stack.

    @param i_fMinVal [in]
        New minimum scale value in world coordinates (physical value).
    @param i_fMaxVal [in]
        New maximum scale value in world coordinates (physical value).
    @param i_pUnit [in]
        You may pass a pointer to a unit if the given minimum and maximum value
        are not in the scales current unit.
*/
void CDiagScale::zoomIn( double i_fMinVal, double i_fMaxVal, CUnit* i_pUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fMinVal) + ", " + QString::number(i_fMaxVal);
        if (i_pUnit != nullptr ) strMthInArgs += ", " + i_pUnit->symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomIn",
        /* strAddInfo   */ strMthInArgs );

    CUnit unit = m_scale.unit();

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }

    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    bool bConvertible = true;

    if( unit != m_scale.unit() )
    {
        // I want to see if the physical unit is convertible before pushing
        // the new scale onto the stack. Otherwise the "unit not convertible"
        // exception would be thrown on poping the scale from the stack.
        // And thats too late to find the cause and source of the exception.
        bConvertible = unit.isConvertible(m_scale.unit(), 0.0);
    }
    if( bConvertible )
    {
        CScale scaleZoom = m_scale;

        scaleZoom.setMinVal(i_fMinVal);
        scaleZoom.setMaxVal(i_fMaxVal);
        scaleZoom.setUnit(unit);

        pushScale(m_scale);
        setScale(scaleZoom); // sets the flag calculated to false and calls "scaleChanged" of diagram
    }
}

//------------------------------------------------------------------------------
/*! @brief Sets the scaling to the last scale scale values of the zoom stack or
           to the first scale value of the zoom stack, if i_bComplete is set to true.

    Depending on the passed i_bComplete flag either just the most recently poped
    value of the zoom stack is removed or the wholde zoom stack is cleared.

    @param i_bComplete [in]
        Set to true if the first scale values should be restored from zoom stack.
*/
void CDiagScale::zoomOut( bool i_bComplete )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "Complete: " + bool2Str(i_bComplete);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "zoomOut",
        /* strAddInfo   */ strMthInArgs );

    CScale scale = m_scale;

    if( i_bComplete )
    {
        if( m_iZoomCount > 0 && m_pZoomStackFirst != nullptr )
        {
            scale = m_pZoomStackFirst->m_scale;

            try
            {
                clearScaleStack();
            }
            catch(...)
            {
            }
        }
        m_pZoomStackFirst = nullptr;
        m_pZoomStackLast = nullptr;
    }
    else if( m_iZoomCount > 0 )
    {
        scale = popScale();
    }
    if( scale != m_scale )
    {
        setScale(scale); // sets the flag calculated to false and calls "scaleChanged" of diagram
    }
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Updates the internal data of the scale object.
*/
void CDiagScale::update()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "update",
        /* strAddInfo   */ "" );

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) )
    {
        QString strMthRuntimeInfo =
            "ScaleIsValid: " + bool2Str(isScaleValid()) +
            ", DivLinesCalculated: " + bool2Str(m_divLines.divLinesCalculated());
    }

    if (!m_divLines.divLinesCalculated())
    {
        m_divLines.update();

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
    }
}

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Clones the object and adds it to the target diagram.

    @param i_pDiagramTrg [in]
        Reference to target diagram the cloned object should be added to.

    @return Pointer to cloned object.
*/
CDiagScale* CDiagScale::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    CDiagScale* pDiagScaleCloned = new CDiagScale(
        /* strObjName */ m_strObjName,
        /* scaleDir   */ m_divLines.scaleDir(),
        /* scale      */ m_scale );

    if( m_pZoomStackFirst != nullptr )
    {
        pDiagScaleCloned->m_scale = m_pZoomStackFirst->m_scale;
    }

    pDiagScaleCloned->m_divLines = m_divLines;

    //for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    //{
    //    pDiagScaleCloned->m_ariDivLineDistMin_px[iLayer] = m_ariDivLineDistMin_px[iLayer];
    //}

    //pDiagScaleCloned->m_uMainDivLineCount = m_uMainDivLineCount;
    //if( pDiagScaleCloned->m_uMainDivLineCount > 0 )
    //{
    //    pDiagScaleCloned->m_arfMainDivLineVal = new double[pDiagScaleCloned->m_uMainDivLineCount];
    //    pDiagScaleCloned->m_arfMainDivLinePix = new double[pDiagScaleCloned->m_uMainDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScaleCloned->m_uMainDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScaleCloned->m_arfMainDivLineVal[idxDivLine] = m_arfMainDivLineVal[idxDivLine];
    //        pDiagScaleCloned->m_arfMainDivLinePix[idxDivLine] = m_arfMainDivLinePix[idxDivLine];
    //    }
    //}

    //pDiagScaleCloned->m_uSubDivLineCount = m_uSubDivLineCount;
    //if( pDiagScaleCloned->m_uSubDivLineCount > 0 )
    //{
    //    pDiagScaleCloned->m_arfSubDivLineVal = new double[pDiagScaleCloned->m_uSubDivLineCount];
    //    pDiagScaleCloned->m_arfSubDivLinePix = new double[pDiagScaleCloned->m_uSubDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScaleCloned->m_uSubDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScaleCloned->m_arfSubDivLineVal[idxDivLine] = m_arfSubDivLineVal[idxDivLine];
    //        pDiagScaleCloned->m_arfSubDivLinePix[idxDivLine] = m_arfSubDivLinePix[idxDivLine];
    //    }
    //}

    i_pDiagramTrg->addDiagScale(pDiagScaleCloned);

    return pDiagScaleCloned;

} // clone

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Constructor for an entry of the internal zoom stack.

    @param i_scale Scaling values.
*/
CDiagScale::SZoomStackEntry::SZoomStackEntry( const CScale& i_scale ) :
//------------------------------------------------------------------------------
    m_scale(i_scale),
    m_pStackEntryNext(nullptr),
    m_pStackEntryPrev(nullptr)
{
} // ctor

/*==============================================================================
protected:  // instance memthods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to push the given scale onto the zoom stack.

    @param i_scale [in]
        Scale to be pushed.
*/
void CDiagScale::pushScale( const CScale& i_scale )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "pushScale",
        /* strAddInfo   */ strMthInArgs );

    SZoomStackEntry* pEntry = new SZoomStackEntry(i_scale);

    // First object ..
    if( m_pZoomStackFirst == nullptr )
    {
        pEntry->m_pStackEntryPrev = nullptr;
        pEntry->m_pStackEntryNext = nullptr;

        m_pZoomStackFirst = pEntry;
        m_pZoomStackLast  = pEntry;
    }
    // Not the first object ...
    else if( m_pZoomStackLast != nullptr )
    {
        m_pZoomStackLast->m_pStackEntryNext = pEntry;

        pEntry->m_pStackEntryPrev = m_pZoomStackLast;
        pEntry->m_pStackEntryNext = nullptr;

        m_pZoomStackLast = pEntry;
    }
    m_iZoomCount++;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to pop the last scale from the zoom stack.

    @return Scale poped from stack.
*/
CScale CDiagScale::popScale()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "popScale",
        /* strAddInfo   */ "" );

    if( m_iZoomCount == 0 || m_pZoomStackFirst == nullptr || m_pZoomStackLast == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultListIsEmpty);
    }

    SZoomStackEntry* pEntry = m_pZoomStackLast;
    CScale scale = pEntry->m_scale;

    // Just one object on stack ...
    if( m_pZoomStackFirst == m_pZoomStackLast )
    {
        m_pZoomStackFirst = nullptr;
        m_pZoomStackLast  = nullptr;
    }
    // Not the first object ...
    else
    {
        pEntry->m_pStackEntryPrev->m_pStackEntryNext = nullptr;
        m_pZoomStackLast = pEntry->m_pStackEntryPrev;
    }
    delete pEntry;
    pEntry = nullptr;
    m_iZoomCount--;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(scale.toString());
    }
    return scale;
}

//------------------------------------------------------------------------------
/*! @brief Internal auxiliary method to clear the zoom stack.

    @return Top most scale entry of zoom stack.
*/
CScale CDiagScale::clearScaleStack()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDatailLevel */ EMethodTraceDetailLevel::ArgsNormal,
        /* strMethod    */ "clearScaleStack",
        /* strAddInfo   */ "" );

    if( m_iZoomCount == 0 || m_pZoomStackFirst == nullptr || m_pZoomStackLast == nullptr )
    {
        throw CException(__FILE__,__LINE__,EResultListIsEmpty);
    }

    SZoomStackEntry* pEntry = m_pZoomStackFirst;
    SZoomStackEntry* pEntryPrev = nullptr;
    CScale scale = pEntry->m_scale;

    pEntry = m_pZoomStackLast;
    while( pEntry != nullptr )
    {
        pEntryPrev = pEntry->m_pStackEntryPrev;
        delete pEntry;
        pEntry = nullptr;
        pEntry = pEntryPrev;
    }
    m_pZoomStackFirst = nullptr;
    m_pZoomStackLast  = nullptr;
    m_iZoomCount = 0;

    if (mthTracer.areMethodCallsActive(EMethodTraceDetailLevel::ArgsNormal)) {
        mthTracer.setMethodReturn(scale.toString());
    }
    return scale;
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Internal method trace emitting the signal.
*/
void CDiagScale::emit_spacingChanged( ESpacing i_spacing )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumSpacing(i_spacing).toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_spacingChanged",
        /* strAddInfo   */ strMthInArgs );

    emit spacingChanged(i_spacing);
}

//------------------------------------------------------------------------------
/*! @brief Internal method trace emitting the signal.
*/
void CDiagScale::emit_scaleChanged( const CScale& i_scale )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_scale.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_scaleChanged",
        /* strAddInfo   */ strMthInArgs );

    emit scaleChanged(i_scale);
}

//------------------------------------------------------------------------------
/*! @brief Internal method trace emitting the signal.
*/
void CDiagScale::emit_geometryChanged( int i_iMinVal_px, int i_iMaxVal_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iMinVal_px) + ", " + QString::number(i_iMaxVal_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_geometryChanged",
        /* strAddInfo   */ strMthInArgs );

    emit geometryChanged(i_iMinVal_px, i_iMaxVal_px);
}

//------------------------------------------------------------------------------
/*! @brief Internal method trace emitting the signal.
*/
void CDiagScale::emit_divLineDistMinPixChanged( EDivLineLayer i_layer, int i_iDivLineDistMin_px )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = CEnumDivLineLayer(i_layer).toString() + ", " + QString::number(i_iDivLineDistMin_px);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObjUpdate,
        /* iDatailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "emit_divLineDistMinPixChanged",
        /* strAddInfo   */ strMthInArgs );

    emit divLineDistMinPixChanged(i_layer, i_iDivLineDistMin_px);
}
