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
protected: // class members
==============================================================================*/

bool   CDiagScale::s_bClassInitialised = false;
double CDiagScale::s_arfScaleRangeFacPixDivValLog[9];

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
    m_scaleDir(i_scaleDir),
    m_spacing(ESpacing::Linear),
    m_scale(i_scale),
    m_iMinVal_px(0),
    m_iMaxVal_px(0),
    m_ariDivLineDistMin_px(CEnumDivLineLayer::count(), 0),
    m_ariDivLineCount(CEnumDivLineLayer::count(), 0),
    m_fDivDistMinVal(0.0),
    m_ararfDivLineVal(CEnumDivLineLayer::count(), QVector<double>()),
    m_ararfDivLine_px(CEnumDivLineLayer::count(), QVector<double>()),
    m_bDivLinesCalculated(false),
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

    if( areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal) )
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

    m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] = 50;

    if( !s_bClassInitialised )
    {
        double fFacLin = 2.0;

        // In logarithmic scale each decade may be divided into 9 sections indicating
        // the values 1.0*10^x, 2.0*10^x, .... , 9.0*10^x, 10.0*10^x with x as an
        // signed integer value. The width of the sections is decreasing from beginning
        // of the first value of the decade to the last value of the decade (non
        // equidistant grid). The portion of each sector is the same for all visible decades.
        for( int idxSection = 0; idxSection < 9; idxSection++, fFacLin += 1.0 )
        {
            s_arfScaleRangeFacPixDivValLog[idxSection] = log10(fFacLin) - log10(fFacLin-1.0);
        }
        s_bClassInitialised = true;
    }

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
    m_scaleDir = static_cast<EScaleDir>(0);
    m_spacing = static_cast<ESpacing>(0);
    //m_scale;
    m_iMinVal_px = 0;
    m_iMaxVal_px = 0;
    //m_ariDivLineDistMin_px.clear();
    //m_ariDivLineCount.clear();
    m_fDivDistMinVal = 0.0;
    //m_ararfDivLineVal.clear();
    //m_ararfDivLine_px.clear();
    m_bDivLinesCalculated = false;
    m_iZoomCount = 0;
    m_pZoomStackFirst = nullptr; // passierte zwar schon in clearScaleStack, weiss aber lint nicht
    m_pZoomStackLast = nullptr;  // passierte zwar schon in clearScaleStack, weiss aber lint nicht

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
    return m_scaleDir;
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

    if (m_spacing != i_spacing.enumerator())
    {
        m_spacing = i_spacing.enumerator();

        m_bDivLinesCalculated = false;

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_spacingChanged(m_spacing);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the spacing of the scale which could be either Linear or Logarithmic.

    @return Spacing of the scale.
*/
ESpacing CDiagScale::getSpacing() const
//------------------------------------------------------------------------------
{
    return m_spacing;
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
    else if( m_spacing == ESpacing::Logarithmic )
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

        m_bDivLinesCalculated = false;

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

        m_bDivLinesCalculated = false;

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

        m_bDivLinesCalculated = false;

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

        m_bDivLinesCalculated = false;

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
/*! @brief Returns the maximum scale value (world coordinates).

    @return Maximum scale value.
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

        m_bDivLinesCalculated = false;

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

        m_bDivLinesCalculated = false;

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

    if ((m_iMinVal_px != i_iMinVal_px) || (m_iMaxVal_px != i_iMaxVal_px))
    {
        m_iMinVal_px = i_iMinVal_px;
        m_iMaxVal_px = i_iMaxVal_px;

        m_bDivLinesCalculated = false;

        if (m_pDiagram != nullptr)
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_geometryChanged(m_iMinVal_px, m_iMaxVal_px);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum geometry value in pixels.

    @Return Minimum value in pixels.
*/
int CDiagScale::getMinValPix() const
//------------------------------------------------------------------------------
{
    return m_iMinVal_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the maximum geometry value in pixels.

    @Return Maximum value in pixels.
*/
int CDiagScale::getMaxValPix() const
//------------------------------------------------------------------------------
{
    return m_iMaxVal_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the range in pixels.

    @Return Range in pixels.
*/
int CDiagScale::getRangePix() const
//------------------------------------------------------------------------------
{
    int iScaleRangePix = 0;

    if( m_iMaxVal_px > m_iMinVal_px )
    {
        iScaleRangePix = m_iMaxVal_px - m_iMinVal_px + 1; //lint !e834
    }
    else if( m_iMinVal_px > m_iMaxVal_px )
    {
        iScaleRangePix = m_iMinVal_px - m_iMaxVal_px + 1; //lint !e834
    }
    return iScaleRangePix;
}

/*==============================================================================
public: // instance methods (calculation of division lines)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the minimum distance between two division lines in pixels.

    The final distance is calculated by the scale object so that the
    division line values become a whole number of a decimal power.

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

    if( m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()] != i_iDistMinPix )
    {
        m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()] = i_iDistMinPix;

        m_bDivLinesCalculated = false;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
        emit_divLineDistMinPixChanged(i_eLayer.enumerator(), i_iDistMinPix);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the minimum distance between two division lines in pixels.

    @param i_eLayer [in]
        Range [Main, Sub]
        Layer for which the value should be applied.

    @return Minimum distance between two division lines in pixels.
*/
int CDiagScale::getDivLineDistMinPix( const CEnumDivLineLayer& i_eLayer ) const
//------------------------------------------------------------------------------
{
    return m_ariDivLineDistMin_px[i_eLayer.enumeratorAsInt()];
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
    return m_bDivLinesCalculated;
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
    int iDivLineCount = 0;

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        iDivLineCount = m_ariDivLineCount[i_eLayer.enumeratorAsInt()];
    }
    return iDivLineCount;
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
double CDiagScale::getDivLineDistMin( const CEnumDivLineLayer& i_eLayer, const PhysVal::CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    double fDivLineDistMin = m_scale.rangeVal().getVal();

    int idxLayerMin = 0;
    int idxLayerMax = CEnumDivLineLayer::count()-1;

    if (i_eLayer.isValid())
    {
        idxLayerMin = i_eLayer.enumeratorAsInt();
        idxLayerMax = i_eLayer.enumeratorAsInt();
    }
    for (int idxLayer = idxLayerMin; idxLayer <= idxLayerMax; ++idxLayer)
    {
        for (int idxDivLine = 1; idxDivLine < m_ariDivLineCount[idxLayer]; ++idxDivLine)
        {
            double fDivLineValPrev = m_ararfDivLineVal[idxLayer][idxDivLine-1];
            double fDivLineValCurr = m_ararfDivLineVal[idxLayer][idxDivLine];
            if ((fDivLineValCurr - fDivLineValPrev) < fDivLineDistMin)
            {
                fDivLineDistMin = fDivLineValCurr - fDivLineValPrev;
                if (m_spacing == ESpacing::Linear)
                {
                    break;
                }
            }
        }
    }
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
double CDiagScale::getDivLineVal( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine, const CUnit* i_pUnit ) const
//------------------------------------------------------------------------------
{
    CUnit unit = m_scale.unit();

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    double fDivLineVal = 0.0;

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        if( i_idxDivLine < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
        {
            fDivLineVal = m_ararfDivLineVal[i_eLayer.enumeratorAsInt()][i_idxDivLine];
        }
        if( unit != m_scale.unit() )
        {
            fDivLineVal = m_scale.unit().convertValue(fDivLineVal, unit);
        }
    }
    return fDivLineVal;
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
    double fDivLine_px = 0.0;

    if( getRangePix() > 1 && isScaleValid() && areDivLinesCalculated() )
    {
        if( i_idxDivLine < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
        {
            fDivLine_px = m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine];
        }
    }
    return fDivLine_px;
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
double CDiagScale::getDivLineDistPix( const CEnumDivLineLayer& i_eLayer, int i_idxDivLine1, int i_idxDivLine2 ) const
//------------------------------------------------------------------------------
{
    double fDivLineDistPix = 0.0;

    if( i_idxDivLine1 < m_ariDivLineCount[i_eLayer.enumeratorAsInt()]
     && i_idxDivLine2 < m_ariDivLineCount[i_eLayer.enumeratorAsInt()] )
    {
        fDivLineDistPix = m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine2]
                        - m_ararfDivLine_px[i_eLayer.enumeratorAsInt()][i_idxDivLine1];
    }
    return fDivLineDistPix;
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

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__,__LINE__,EResultDifferentPhysSizes);
    }

    double fPix = static_cast<double>(m_iMinVal_px);

    if( getRangePix() > 1 && isScaleValid() )
    {
        double fScaleValMin = m_scale.minVal().getVal(unit);
        double fScaleValMax = m_scale.maxVal().getVal(unit);

        // Convert value into unit of division line values.
        double fVal = i_fVal;
        if( unit != m_scale.unit() )
        {
            fVal = unit.convertValue(i_fVal, m_scale.unit());
        }

        // At the minimum scale value ..
        if( fVal == fScaleValMin )
        {
            fPix = m_iMinVal_px;
        }
        // At the maximum scale value ..
        else if( fVal == fScaleValMax )
        {
            fPix = m_iMaxVal_px;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            // Calculate range and resolution of one pixel:
            if( m_spacing == ESpacing::Logarithmic )
            {
                fScaleValMin = log10(fScaleValMin);
                fScaleValMax = log10(fScaleValMax);
            }

            double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
            double fScalePixRange = fabs(m_iMaxVal_px-m_iMinVal_px)+1;
            double fPixRes        = (fScalePixRange-1) / fScaleValRange;

            bool bDivLineHit = false;

            // Get the nearest division line.
            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ariDivLineCount[iLayer] > 1 )
                {
                    for( int idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                    {
                        double fDivLineVal1 = m_ararfDivLineVal[iLayer][idxDivLine];
                        double fDivLineVal2 = m_ararfDivLineVal[iLayer][idxDivLine+1];

                        if( fVal >= (fDivLineVal1-DBL_EPSILON) && fVal <= (fDivLineVal1+DBL_EPSILON) )
                        {
                            fPix = static_cast<int>(m_ararfDivLine_px[iLayer][idxDivLine]);
                            bDivLineHit = true;
                            break;
                        }
                        else if( fVal >= (fDivLineVal2-DBL_EPSILON) && fVal <= (fDivLineVal2+DBL_EPSILON) )
                        {
                            fPix = static_cast<int>(m_ararfDivLine_px[iLayer][idxDivLine+1]);
                            bDivLineHit = true;
                            break;
                        }
                        else if( fVal > fDivLineVal1 && fVal < fDivLineVal2 )
                        {
                            break;
                        }
                    }
                }
                else if( m_ariDivLineCount[iLayer] == 1 )
                {
                    double fDivLineVal1 = m_ararfDivLineVal[iLayer][0];

                    if( fVal == fDivLineVal1 )
                    {
                        fPix = static_cast<int>(m_ararfDivLine_px[iLayer][0]);
                        bDivLineHit = true;
                    }
                }
                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                if( m_spacing == ESpacing::Logarithmic )
                {
                    fVal = log10(fVal);
                }

                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDir::X:
                    {
                        fPix = static_cast<double>(m_iMinVal_px) + fPixRes*(fVal-fScaleValMin); //lint !e834
                        break;
                    }
                    case EScaleDir::Y:
                    {
                        fPix = static_cast<double>(m_iMinVal_px) - fPixRes*(fVal-fScaleValMin); //lint !e834
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            } // Neither hit a main nor a sub division line ..
        } // Somewhere between minimum and maximum scale ..
    } // if( isScaleValid() )

    return static_cast<int>(fPix);

} // getValPix

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

    if( i_pUnit != nullptr )
    {
        unit = *i_pUnit;
    }
    if( !areOfSameUnitGroup(m_scale.unit(), unit) )
    {
        throw CException(__FILE__, __LINE__, EResultDifferentPhysSizes);
    }

    double fVal = m_scale.minVal().getVal(unit);

    if( getRangePix() > 1 && isScaleValid() )
    {
        double fScaleValMin = m_scale.minVal().getVal(unit);
        double fScaleValMax = m_scale.maxVal().getVal(unit);

        // Calculate range and resolution of one pixel:
        if( m_spacing == ESpacing::Logarithmic )
        {
            fScaleValMin = log10(fScaleValMin);
            fScaleValMax = log10(fScaleValMax);
        }

        double fScaleValRange = fabs(fScaleValMax-fScaleValMin);
        double fScalePixRange = fabs(m_iMaxVal_px-m_iMinVal_px)+1;
        double fPixRes        = fScaleValRange / fScalePixRange;

        // At the minimum scale value ..
        if( i_fPix == static_cast<double>(m_iMinVal_px) )
        {
            fVal = fScaleValMin;
        }
        // At the maximum scale value ..
        else if( i_fPix == static_cast<double>(m_iMaxVal_px) )
        {
            fVal = fScaleValMax;
        }

        // Somewhere between minimum and maximum scale ..
        else
        {
            bool bDivLineHit = false;

            // Get the nearest division line. This is necessary to exactly return the value
            // at a grid line if the mouse cursor is positioned on a grid line.
            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                if( m_ariDivLineCount[iLayer] > 1 )
                {
                    for( int idxDivLine = 0; idxDivLine < m_ariDivLineCount[iLayer]-1; idxDivLine++ )
                    {
                        double fDivLinePix1 = m_ararfDivLine_px[iLayer][idxDivLine];
                        double fDivLinePix2 = m_ararfDivLine_px[iLayer][idxDivLine+1];

                        if( i_fPix >= (fDivLinePix1-DBL_EPSILON) && i_fPix <= (fDivLinePix1+DBL_EPSILON) )
                        {
                            fVal = m_ararfDivLineVal[iLayer][idxDivLine];
                            bDivLineHit = true;
                            break;
                        }
                        else if( i_fPix >= (fDivLinePix2-DBL_EPSILON) && i_fPix <= (fDivLinePix2+DBL_EPSILON) )
                        {
                            fVal = m_ararfDivLineVal[iLayer][idxDivLine+1];
                            bDivLineHit = true;
                            break;
                        }
                        else if( i_fPix > fDivLinePix1 && i_fPix < fDivLinePix2 )
                        {
                            break;
                        }
                    }
                }
                else if( m_ariDivLineCount[iLayer] == 1 )
                {
                    double fDivLinePix1 = m_ararfDivLine_px[iLayer][0];

                    if( i_fPix == fDivLinePix1 )
                    {
                        fVal = static_cast<int>(m_ararfDivLineVal[iLayer][0]);
                        bDivLineHit = true;
                    }
                }
                if( bDivLineHit )
                {
                    break;
                }
            } // for( iLayer < CEnumDivLineLayer::count() )

            // Did not hit a division line ..
            if( !bDivLineHit )
            {
                // Calculate value as distance to left grid line:
                switch( m_scaleDir )
                {
                    case EScaleDir::X:
                    {
                        fVal = fScaleValMin + fPixRes*(i_fPix-static_cast<double>(m_iMinVal_px)+1); //lint !e834
                        break;
                    }
                    case EScaleDir::Y:
                    {
                        fVal = fScaleValMin + fPixRes*(static_cast<double>(m_iMinVal_px)-i_fPix+1); //lint !e834
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                if( m_spacing == ESpacing::Logarithmic )
                {
                    fVal = pow(10.0,fVal);
                }
            } // Neither hit a main nor a sub division line ..
        } // Somewhere between minimum and maximum scale ..

        if( m_spacing == ESpacing::Linear )
        {
            fPixRes = Math::round2LowerDecade(fPixRes);
        }
        fVal = Math::round2Resolution(fVal, fPixRes);

    } // if( getRangePix() > 1 && isScaleValid() )

    return fVal;

} // getVal

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
        m_pZoomStackFirst = nullptr; // passierte zwar schon in clearScaleStack, weiss aber lint nicht
        m_pZoomStackLast = nullptr;  // passierte zwar schon in clearScaleStack, weiss aber lint nicht
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
            ", DivLinesCalculated: " + bool2Str(m_bDivLinesCalculated);
    }

    // Division lines:
    //----------------

    if( !m_bDivLinesCalculated )
    {
        double fScaleMinVal = m_scale.minVal().getVal();
        double fScaleMaxVal = m_scale.maxVal().getVal();

        for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
        {
            m_ariDivLineCount[iLayer] = 0;
            m_ararfDivLineVal[iLayer].clear();
            m_ararfDivLine_px[iLayer].clear();
        }
        m_fDivDistMinVal = 0.0;

        int iScaleRangePix = getRangePix();

        if( iScaleRangePix <= 1 || m_ariDivLineDistMin_px[static_cast<int>(EDivLineLayer::Main)] < 2 )
        {
        }
        else if( !isScaleValid() )
        {
            m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)] = 2;
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)].resize(
                m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)]);
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)].resize(
                m_ariDivLineCount[static_cast<int>(EDivLineLayer::Main)]);
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)][0] = fScaleMinVal;
            m_ararfDivLineVal[static_cast<int>(EDivLineLayer::Main)][1] = fScaleMaxVal;
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)][0] = m_iMinVal_px;
            m_ararfDivLine_px[static_cast<int>(EDivLineLayer::Main)][1] = m_iMaxVal_px;
        }

        // Calculate main and sub division lines
        //--------------------------------------

        else
        {
            QVector<int> ariDivLineCount(CEnumDivLineLayer::count(), 0);

            m_fDivDistMinVal = m_scale.rangeVal().getVal();

            // Calculate division lines for linear spacing
            //--------------------------------------------

            if( m_spacing == ESpacing::Linear )
            {
                double fDivLineDistValMin = fScaleMinVal;
                double fDivLineDistValMax = fScaleMaxVal;
                QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count());
                QVector<double> arfDivLineFirstVal(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistVal(CEnumDivLineLayer::count());
                QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count());

                for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
                {
                    if (m_ariDivLineDistMin_px[iLayer] > 0)
                    {
                        // Calculate optimized distance between two grid lines:
                        ariDivLineCountTmp[iLayer] = Math::getDivLines4LinSpacing(
                            /* fScaleMinVal          */ fDivLineDistValMin,
                            /* fScaleMaxVal          */ fDivLineDistValMax,
                            /* iScaleRangePix        */ iScaleRangePix,
                            /* fDivLineDistMinVal    */ 0.0,
                            /* iDivLineDistMinPix    */ m_ariDivLineDistMin_px[iLayer],
                            /* bUseDivLineDecFac25   */ true,
                            /* pfDivLineFirstVal     */ &arfDivLineFirstVal[iLayer],
                            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[iLayer],
                            /* pfDivLineDistVal      */ &arfDivLineDistVal[iLayer],
                            /* pfDivLineDistPix      */ &arfDivLineDistPix[iLayer],
                            /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);

                        if (m_fDivDistMinVal > arfDivLineDistVal[iLayer] )
                        {
                            m_fDivDistMinVal = arfDivLineDistVal[iLayer];
                        }

                        // Store the calculated results:
                        if( ariDivLineCountTmp[iLayer] > 0 )
                        {
                            if( iLayer == 0 )
                            {
                                // Count visible division lines for the main grid lines:
                                ariDivLineCount[iLayer] = ariDivLineCountTmp[iLayer];

                                m_ararfDivLineVal[iLayer].resize(ariDivLineCount[iLayer]);
                                m_ararfDivLine_px[iLayer].resize(ariDivLineCount[iLayer]);

                                // Calculate pixel values and store the calculated results of the main grid lines:
                                for( int idxDivLine = 0; idxDivLine < ariDivLineCount[iLayer]; idxDivLine++ )
                                {
                                    double fVal = arfDivLineFirstVal[iLayer] + idxDivLine*arfDivLineDistVal[iLayer];
                                    if( fabs(fVal) < DBL_EPSILON )
                                    {
                                        fVal = 0.0;
                                    }
                                    m_ararfDivLineVal[iLayer][idxDivLine] = fVal;
                                    m_ararfDivLine_px[iLayer][idxDivLine] = getValPix(fVal);
                                }
                            }
                            else // if (iLayer != 0)
                            {
                                // Count visible sub division lines:
                                double fPrevLayerDivLineVal = 0.0;
                                modf(fScaleMinVal/arfDivLineDistVal[iLayer-1], &fPrevLayerDivLineVal);
                                fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                                double fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                                while( fCurrLayerDivLineVal <= fScaleMaxVal )
                                {
                                    for( int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++ )
                                    {
                                        fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                                        if( fabs(fCurrLayerDivLineVal) < DBL_EPSILON )
                                        {
                                            fCurrLayerDivLineVal = 0.0;
                                        }
                                        if( fCurrLayerDivLineVal >= fScaleMinVal && fCurrLayerDivLineVal <= fScaleMaxVal )
                                        {
                                            ariDivLineCount[iLayer]++;
                                        }
                                    }
                                    fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                                }

                                // Calculate pixel values and store the calculated results of the sub grid lines:
                                if( ariDivLineCount[iLayer] > 0 )
                                {
                                    m_ararfDivLineVal[iLayer].resize(ariDivLineCount[iLayer]);
                                    m_ararfDivLine_px[iLayer].resize(ariDivLineCount[iLayer]);

                                    modf(fScaleMinVal/arfDivLineDistVal[iLayer-1], &fPrevLayerDivLineVal);
                                    fPrevLayerDivLineVal *= arfDivLineDistVal[iLayer-1];
                                    fCurrLayerDivLineVal = fPrevLayerDivLineVal;
                                    int idxDivLine = 0;
                                    while( fCurrLayerDivLineVal <= fScaleMaxVal )
                                    {
                                        for( int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[iLayer]-1; idxDivLineTmp++ )
                                        {
                                            fCurrLayerDivLineVal = fPrevLayerDivLineVal + idxDivLineTmp*arfDivLineDistVal[iLayer];
                                            if( fabs(fCurrLayerDivLineVal) < DBL_EPSILON )
                                            {
                                                fCurrLayerDivLineVal = 0.0;
                                            }
                                            if( fCurrLayerDivLineVal >= fScaleMinVal && fCurrLayerDivLineVal <= fScaleMaxVal )
                                            {
                                                if( idxDivLine < ariDivLineCount[iLayer] )
                                                {
                                                    m_ararfDivLineVal[iLayer][idxDivLine] = fCurrLayerDivLineVal;
                                                    m_ararfDivLine_px[iLayer][idxDivLine] = getValPix(fCurrLayerDivLineVal);
                                                    idxDivLine++;
                                                }
                                            }
                                        }
                                        fPrevLayerDivLineVal += arfDivLineDistVal[iLayer-1];
                                    }
                                }
                            } // if( iLayer != 0 )
                        } // if( ariDivLineCount[iLayer] > 0 )

                        // If there is a next layer ...
                        if( iLayer < CEnumDivLineLayer::count()-1 )
                        {
                            if( m_ariDivLineDistMin_px[iLayer+1] > 1 && m_ariDivLineDistMin_px[iLayer] > 2*m_ariDivLineDistMin_px[iLayer+1] )
                            {
                                fDivLineDistValMin = 0.0;
                                fDivLineDistValMax = arfDivLineDistVal[iLayer];
                            }
                            else
                            {
                                // Next layer is empty ...
                                break;
                            }
                        }
                    } // if (m_ariDivLineDistMin_px[iLayer] > 0)
                } // for( iLayer < CEnumDivLineLayer::count() )
            } // if( m_spacing == ESpacingLinear )

            // Calculate division lines for logarithmic spacing
            //-------------------------------------------------

            else // if( m_spacing == ESpacingLogarithmic )
            {
                // Sub grid (non equidistant) within main grid (equidistant) NOT including "left" and
                // "right" main grid lines. Please note that in a logarithmic scale one decade comprises
                // 8 sub grid lines, e.g. 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 (the values 1.0 and
                // 10.0 belong to the main grid):

                QVector<int> ariDivLineCountMax(CEnumDivLineLayer::count(), 0);
                QVector<int> ariDivLineDistMinPix(CEnumDivLineLayer::count(), 0);
                QVector<double> arfDivLineFirstValLog(CEnumDivLineLayer::count(), 0.0);
                QVector<double> arfDivLineDistFirstPix(CEnumDivLineLayer::count(), 0.0);
                QVector<double> arfDivLineDistValLog(CEnumDivLineLayer::count(), 0.0);
                QVector<double> arfDivLineDistPix(CEnumDivLineLayer::count(), 0.0);
                QVector<double> arfDivLineValLin(CEnumDivLineLayer::count(), 0.0);
                QVector<double> arfDivLineValLog(CEnumDivLineLayer::count(), 0.0);
                QVector<int> ariDivLineCountTmp(CEnumDivLineLayer::count(), 0);

                double fScaleMinValLog = log10(fScaleMinVal);
                double fScaleMaxValLog = log10(fScaleMaxVal);
                double fScaleRangeValLog = fScaleMaxValLog - fScaleMinValLog;

                // Normalize the axis range to engineering format (e.g. 4.234e9):
                int iScaleMinValLinNormExp = static_cast<int>(fScaleMinValLog);
                double fScaleMinValLinNorm = fScaleMinVal/pow(10.0,iScaleMinValLinNormExp);
                while( fScaleMinValLinNorm < 1.0 )
                {
                    iScaleMinValLinNormExp--;
                    fScaleMinValLinNorm *= 10.0;
                }
                while( fScaleMinValLinNorm >= 10.0 )
                {
                    iScaleMinValLinNormExp++;
                    fScaleMinValLinNorm /= 10.0;
                }
                double fScaleMinValLinNormLeading = 0.0;
                double fScaleMinValLinNormFract = modf(fScaleMinValLinNorm, &fScaleMinValLinNormLeading);

                int iScaleMaxValLinNormExp = static_cast<int>(fScaleMaxValLog);
                double fScaleMaxValLinNorm = fScaleMaxVal/pow(10.0,iScaleMaxValLinNormExp);
                while( fScaleMaxValLinNorm < 1.0 )
                {
                    iScaleMaxValLinNormExp--;
                    fScaleMaxValLinNorm *= 10.0;
                }
                while( fScaleMaxValLinNorm >= 10.0 )
                {
                    iScaleMaxValLinNormExp++;
                    fScaleMaxValLinNorm /= 10.0;
                }
                double fScaleMaxValLinNormLeading = 0.0;
                double fScaleMaxValLinNormFract = modf(fScaleMaxValLinNorm, &fScaleMaxValLinNormLeading);

                // Extend the axis range by rounding down the scale minimum value and rounding up the
                // scale maximum value so that the following applies:
                // - log(ScaleMinVal) MOD 1 = 0
                // - log(ScaleMaxVal) MOD 1 = 0

                // Round down minimum scale value:
                double fScaleMinValLogExt = iScaleMinValLinNormExp;
                double fScaleMinValLinExt = pow(10.0, fScaleMinValLogExt);

                // Round up maximum scale value:
                double fScaleMaxValLogExt = iScaleMaxValLinNormExp;
                if( fScaleMaxValLinNorm > 1.0 || fScaleMaxValLinNormFract > 0.0 || fScaleMaxValLogExt == fScaleMinValLogExt ) //lint !e777
                {
                    fScaleMaxValLogExt += 1.0;
                }

                // Get extended range:
                //fScaleMinPixExt = m_iMinVal_px - (fScaleMinValLog-fScaleMinValLogExt) * (iScaleRangePix/fScaleRangeValLog);

                // Calculate "main" division lines.
                //---------------------------------

                // For the values of the main division lines the following constraint must apply:
                // - log(MainDivLineVal) MOD 1 = 0
                // The distances between the main division lines are the same (equidistant grid).

                // Calculate maximum number of visible main grid lines:
                ariDivLineCountMax[EDivLineLayerMain] = static_cast<unsigned int>(iScaleMaxValLinNormExp-iScaleMinValLinNormExp)+1;
                if( ariDivLineCountMax[EDivLineLayerMain] > 0 && (fScaleMinValLinNorm > 1.0 || fScaleMinValLinNormFract >= DBL_EPSILON) )
                {
                    ariDivLineCountMax[EDivLineLayerMain]--;
                }

                // If at least one main grid line is visible ...
                if( ariDivLineCountMax[EDivLineLayerMain] > 0 )
                {
                    // Prepare logarithmic scale values for linear spacing:
                    // First assume that the value distance between the main grid lines is 1.0
                    // (the lowest possible value for main grid lines).
                    ariDivLineDistMinPix[EDivLineLayerMain] = static_cast<int>(static_cast<double>(iScaleRangePix) / fScaleRangeValLog);
                    // If the distance of 1.0 between the main grid lines is too small ..
                    if( ariDivLineDistMinPix[EDivLineLayerMain] < m_ariDivLineDistMin_px[EDivLineLayerMain] )
                    {
                        ariDivLineDistMinPix[EDivLineLayerMain] = m_ariDivLineDistMin_px[EDivLineLayerMain];
                    }
                    // If the distance of 1.0 between the main grid lines is too big ..
                    else if( ariDivLineDistMinPix[EDivLineLayerMain] > iScaleRangePix / 2 )
                    {
                        ariDivLineDistMinPix[EDivLineLayerMain] = iScaleRangePix / 2;
                    }

                    // Calculate optimized distance between two main grid lines:
                    ariDivLineCount[EDivLineLayerMain] = Math::getDivLines4LinSpacing(
                        /* fScaleMinVal          */ fScaleMinValLog,
                        /* fScaleMaxVal          */ fScaleMaxValLog,
                        /* iScaleRangePix        */ iScaleRangePix,
                        /* fDivLineDistMinVal    */ 1.0,
                        /* iDivLineDistMinPix    */ ariDivLineDistMinPix[EDivLineLayerMain],
                        /* bUseDivLineDecFac25   */ false,
                        /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[EDivLineLayerMain],
                        /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[EDivLineLayerMain],
                        /* pfDivLineDistVal      */ &arfDivLineDistValLog[EDivLineLayerMain],
                        /* pfDivLineDistPix      */ &arfDivLineDistPix[EDivLineLayerMain],
                        /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);
                }

                // Store the calculated results:
                if( ariDivLineCount[EDivLineLayerMain] > 0 )
                {
                    m_ararfDivLineVal[EDivLineLayerMain].resize(ariDivLineCount[EDivLineLayerMain]);
                    m_ararfDivLine_px[EDivLineLayerMain].resize(ariDivLineCount[EDivLineLayerMain]);

                    for( int idxDivLine = 0; idxDivLine < ariDivLineCount[EDivLineLayerMain]; idxDivLine++ )
                    {
                        arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain] + idxDivLine*arfDivLineDistValLog[EDivLineLayerMain];
                        arfDivLineValLin[EDivLineLayerMain] = pow(10.0,arfDivLineValLog[EDivLineLayerMain]);
                        m_ararfDivLineVal[EDivLineLayerMain][idxDivLine] = arfDivLineValLin[EDivLineLayerMain];
                        m_ararfDivLine_px[EDivLineLayerMain][idxDivLine] = getValPix(arfDivLineValLin[EDivLineLayerMain]);
                    }
                }

                // Calculate "sub" division lines.
                //---------------------------------

                // If not each main division line is visible (e.g. if the scale range enclosed
                // a huge range (many decades) and not each main grid line may be drawn because
                // the distance between two main grid lines would be below the minimum distance
                // between the main grid lines) ..
                if( ariDivLineCount[EDivLineLayerMain] < ariDivLineCountMax[EDivLineLayerMain] )
                {
                    // .. some (or all) of the not yet visible main division lines
                    // become the sub division lines (equidistant grid).

                    // Calculate optimized distance between two sub grid lines:
                    if( m_ariDivLineDistMin_px[EDivLineLayerSub] > 1 && m_ariDivLineDistMin_px[EDivLineLayerMain] > 2*m_ariDivLineDistMin_px[EDivLineLayerSub] )
                    {
                        ariDivLineCountTmp[EDivLineLayerSub] = Math::getDivLines4LinSpacing(
                            /* fScaleMinVal          */ arfDivLineFirstValLog[EDivLineLayerMain],
                            /* fScaleMaxVal          */ arfDivLineFirstValLog[EDivLineLayerMain]+arfDivLineDistValLog[EDivLineLayerMain],
                            /* iScaleRangePix        */ static_cast<int>(arfDivLineDistPix[EDivLineLayerMain]),
                            /* fDivLineDistMinVal    */ 1.0,
                            /* iDivLineDistMinPix    */ m_ariDivLineDistMin_px[EDivLineLayerSub],
                            /* bUseDivLineDecFac25   */ false,
                            /* pfDivLineFirstVal     */ &arfDivLineFirstValLog[EDivLineLayerSub],
                            /* pfDivLineDistFirstPix */ &arfDivLineDistFirstPix[EDivLineLayerSub],
                            /* pfDivLineDistVal      */ &arfDivLineDistValLog[EDivLineLayerSub],
                            /* pfDivLineDistPix      */ nullptr,
                            /* pTrcAdminObj          */ m_pTrcAdminObjUpdate);

                        // Store the calculated results:
                        if( ariDivLineCountTmp[EDivLineLayerSub] > 2 )
                        {
                            // Count visible sub division lines:
                            arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain];
                            if( arfDivLineValLog[EDivLineLayerMain] > fScaleMinValLog )
                            {
                                arfDivLineValLog[EDivLineLayerMain] -= arfDivLineDistValLog[EDivLineLayerMain];
                            }
                            arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain];
                            arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                            while( arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                            {
                                for( int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[EDivLineLayerSub]-1; idxDivLineTmp++ )
                                {
                                    arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain] + idxDivLineTmp*arfDivLineDistValLog[EDivLineLayerSub];
                                    arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                                    if( arfDivLineValLin[EDivLineLayerSub] >= fScaleMinVal && arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                                    {
                                        ariDivLineCount[EDivLineLayerSub]++;
                                    }
                                }
                                arfDivLineValLog[EDivLineLayerMain] += arfDivLineDistValLog[EDivLineLayerMain];
                            }

                            // Calculate pixel values and store the calculated results of the sub grid lines:
                            if( ariDivLineCount[EDivLineLayerSub] > 0 )
                            {
                                m_ararfDivLineVal[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);
                                m_ararfDivLine_px[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);

                                arfDivLineValLog[EDivLineLayerMain] = arfDivLineFirstValLog[EDivLineLayerMain];
                                if( arfDivLineValLog[EDivLineLayerMain] > fScaleMinValLog )
                                {
                                    arfDivLineValLog[EDivLineLayerMain] -= arfDivLineDistValLog[EDivLineLayerMain];
                                }
                                arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain];
                                arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                                int idxDivLine = 0;
                                while( arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                                {
                                    for( int idxDivLineTmp = 1; idxDivLineTmp < ariDivLineCountTmp[EDivLineLayerSub]-1; idxDivLineTmp++ )
                                    {
                                        arfDivLineValLog[EDivLineLayerSub] = arfDivLineValLog[EDivLineLayerMain] + idxDivLineTmp*arfDivLineDistValLog[EDivLineLayerSub];
                                        arfDivLineValLin[EDivLineLayerSub] = pow(10.0,arfDivLineValLog[EDivLineLayerSub]);
                                        if( arfDivLineValLin[EDivLineLayerSub] >= fScaleMinVal && arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                                        {
                                            if( idxDivLineTmp < ariDivLineCount[EDivLineLayerSub] )
                                            {
                                                m_ararfDivLineVal[EDivLineLayerSub][idxDivLine] = arfDivLineValLin[EDivLineLayerSub];
                                                m_ararfDivLine_px[EDivLineLayerSub][idxDivLine] = getValPix(arfDivLineValLin[EDivLineLayerSub]);
                                                idxDivLine++;
                                            }
                                        }
                                    }
                                    arfDivLineValLog[EDivLineLayerMain] += arfDivLineDistValLog[EDivLineLayerMain];
                                }
                            }
                        }
                    }
                }

                // If all main division lines are visible ...
                else
                {
                    //   the decade between the main division lines will be divided into
                    // sub division lines (non equidistant grid).

                    double fScaleRangePixDec = 0.0;

                    // It at least two main division lines are visible ...
                    if( ariDivLineCount[EDivLineLayerMain] > 1 )
                    {
                        fScaleRangePixDec = arfDivLineDistPix[EDivLineLayerMain] / arfDivLineDistValLog[EDivLineLayerMain];
                    }
                    // If no or just one main division line is visible ...
                    else // if( ariDivLineCount[EDivLineLayerMain] == 0 )
                    {
                        // If one main division line is visible it could be that more than one
                        // decade is visible (e.g ranging from 4.0 e9 to 6.0 e10 with one grid
                        // line at 1.0 e10) but it could also be that less than one decade is
                        // visible (e.g. ranging from 6.0 e9 to 4.0 e10 with one grid line at 1.0 e10).

                        // If no division line is visible the axis range is definitely less than
                        // one decade.

                        // Get the extended range:
                        fScaleRangePixDec = iScaleRangePix / fScaleRangeValLog;
                    }

                    // Please note that we cannot be sure that if we would just draw part of
                    // the sub division lines that each of the sub division line will be labeled.
                    // Even worse we don't know which (if any) division line will be labeled.
                    // We could assume that the user agrees in that the division lines will be drawn
                    // as long as the distance between the sub division lines is wide enough and
                    // that always the last division lines will not be drawn. But in this case it could
                    // happen that the distance between the last visible sub division line and the
                    // following main grid line is wider than between the last visible sub division line
                    // and the previous sub division line. And this would look strange ...
                    // So we are just drawing all or none of the sub division lines:

                    // If there is enough space for sub division lines ...
                    if( m_ariDivLineDistMin_px[EDivLineLayerSub] > 1 && s_arfScaleRangeFacPixDivValLog[2] * fScaleRangePixDec >= m_ariDivLineDistMin_px[EDivLineLayerSub] )
                    {
                        // Count the visible sub division lines
                        // (not including main grid line at start and end of section):
                        double fScaleMinValLinDec = fScaleMinValLinExt;
                        arfDivLineValLin[EDivLineLayerSub] = fScaleMinValLinDec;
                        while( arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                        {
                            // Not including main grid line at start and end of section:
                            double fFacLin = 2.0;
                            for( int idxSection = 0; idxSection < 8; idxSection++, fFacLin += 1.0 )
                            {
                                arfDivLineValLin[EDivLineLayerSub] = fFacLin*fScaleMinValLinDec;
                                if( arfDivLineValLin[EDivLineLayerSub] >= fScaleMinVal && arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                                {
                                    ariDivLineCount[EDivLineLayerSub]++;
                                }
                                if( arfDivLineValLin[EDivLineLayerSub] > fScaleMaxVal )
                                {
                                    break;
                                }
                            }
                            fScaleMinValLinDec *= 10.0;
                        }

                        // Calculate pixel values and store the calculated results of the sub grid lines:
                        if( ariDivLineCount[EDivLineLayerSub] > 0 )
                        {
                            m_ararfDivLineVal[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);
                            m_ararfDivLine_px[EDivLineLayerSub].resize(ariDivLineCount[EDivLineLayerSub]);

                            fScaleMinValLinDec = fScaleMinValLinExt;
                            arfDivLineValLin[EDivLineLayerSub] = fScaleMinValLinDec;
                            //fScaleMinPixDec = fScaleMinPixExt;
                            int idxDivLine = 0;
                            while( arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                            {
                                // Not including main grid line at start and end of section:
                                double fFacLin = 2.0;
                                for( int idxSection = 0; idxSection < 8; idxSection++, fFacLin += 1.0 )
                                {
                                    arfDivLineValLin[EDivLineLayerSub] = fFacLin*fScaleMinValLinDec;
                                    if( arfDivLineValLin[EDivLineLayerSub] >= fScaleMinVal && arfDivLineValLin[EDivLineLayerSub] <= fScaleMaxVal )
                                    {
                                        if( idxDivLine < ariDivLineCount[EDivLineLayerSub] )
                                        {
                                            m_ararfDivLineVal[EDivLineLayerSub][idxDivLine] = arfDivLineValLin[EDivLineLayerSub];
                                            m_ararfDivLine_px[EDivLineLayerSub][idxDivLine] = getValPix(arfDivLineValLin[EDivLineLayerSub]);
                                            idxDivLine++;
                                        }
                                    }
                                    if( arfDivLineValLin[EDivLineLayerSub] > fScaleMaxVal )
                                    {
                                        break;
                                    }
                                }
                                fScaleMinValLinDec *= 10.0;
                                //fScaleMinPixDec += fScaleRangePixDec;
                            }
                        }
                    } // If there is enough space for sub division lines ...
                } // If all main division lines are visible ..


                //    // In logarithmic scale each decade may be divided into 1, 2, 4, 5, 10, 20, 40, 50, .. section(s).
                //    // The following algorithm assumes an equidistant division of the decade. As a
                //    // result the distance between the first division lines will be greater than
                //    // SubDivLineDistMinPix and the distance between the last division lines will
                //    // be less than SubDivLineDistMinPix.
                //    iSectionCount = 1;
                //    iSectionCountTmp = 1;
                //    iSectionCountNorm = 1;
                //    iSectionCountNormTmp = 1;
                //    iSectionCountNormExp = 0;
                //    iSectionCountNormExpTmp = 0;
                //    while( static_cast<int>(fScaleRangePixDec/iSectionCountTmp) > m_uSubDivLineDistMinPix )
                //    {
                //        iSectionCount = iSectionCountTmp;
                //        iSectionCountNorm = iSectionCountNormTmp;
                //        iSectionCountNormExp = iSectionCountNormExpTmp;

                //        switch( iSectionCountNorm )
                //        {
                //            case 1:
                //            {
                //                iSectionCountNormTmp = 2;
                //                break;
                //            }
                //            case 2:
                //            {
                //                iSectionCountNormTmp = 4;
                //                break;
                //            }
                //            case 4:
                //            {
                //                iSectionCountNormTmp = 5;
                //                break;
                //            }
                //            case 5:
                //            {
                //                iSectionCountNormExpTmp++;
                //                iSectionCountNormTmp = 1;
                //                break;
                //            }
                //        }
                //        iSectionCountTmp = iSectionCountNormTmp * pow(10.0,static_cast<int>(iSectionCountNormExpTmp));
                //    }

                //    // Count the visible sub division lines:

                //} // If all main division lines are visible ..

                //fSubDivLineDistValLinFact = 1.0 / (iSectionCountNorm*pow(10.0,static_cast<int>(iSectionCountNormExp)));

                //// Calculate count of visible sub division lines:
                //fSubDivLineValLinDecFirst = fScaleMinValLinExt;
                //fSubDivLineValLin = fSubDivLineValLinDecFirst + fSubDivLineDistValLinFact*fSubDivLineValLinDecFirst;
                //while( fSubDivLineValLin < fScaleMaxVal )
                //{
                //    fSubDivLineValLin = 
                //}

                //// Calculate pixel values and store the calculated results of the sub grid lines:
                //if( uSubDivLineCount > 0 )
                //{
                //    m_arfSubDivLineVal = new double[uSubDivLineCount];
                //    m_arfSubDivLinePix = new double[uSubDivLineCount];

                    //udxSubDivLine = 0;
                    //fSubDivLineValLog = static_cast<int>(fScaleMinValLog/fSubDivLineDistValLog)*fSubDivLineDistValLog;
                    //while( fSubDivLineValLog <= fScaleMaxValLog )
                    //{
                    //    fSubDivLineValLin = pow(10.0,fSubDivLineValLog);
                    //    if( fSubDivLineValLin >= fScaleMinVal && fSubDivLineValLin <= fScaleMaxVal )
                    //    {
                    //        if( udxSubDivLine < uSubDivLineCount )
                    //        {
                    //            m_arfSubDivLineVal[udxSubDivLine] = fSubDivLineValLin;
                    //            m_arfSubDivLinePix[udxSubDivLine] = fSubDivLineDistValLog;
                    //            udxSubDivLine++;
                    //        }
                    //    }
                    //    fSubDivLineValLog += fSubDivLineDistValLog;
                    //}
                //}

            } // if( m_spacing == ESpacingLogarithmic )

            for( int iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
            {
                m_ariDivLineCount[iLayer] = ariDivLineCount[iLayer];
            }

        } // if( validScale() && ScaleRangePix > 0 )

        m_bDivLinesCalculated = true;

        if( m_pDiagram != nullptr )
        {
            m_pDiagram->scaleChanged(this);
        }
    } // if( !m_bDivLinesCalculated )

} // update

/*==============================================================================
protected: // overridables
==============================================================================*/

//------------------------------------------------------------------------------
CDiagScale* CDiagScale::clone( CDataDiagram* i_pDiagramTrg ) const
//------------------------------------------------------------------------------
{
    if( i_pDiagramTrg == nullptr )
    {
        return nullptr;
    }

    int iLayer;

    CDiagScale* pDiagScale = new CDiagScale(
        /* strObjName */ m_strObjName,
        /* scaleDir   */ m_scaleDir,
        /* scale      */ m_scale );

    if( m_pZoomStackFirst != nullptr )
    {
        pDiagScale->m_scale = m_pZoomStackFirst->m_scale;
    }

    pDiagScale->m_spacing = m_spacing;
    pDiagScale->m_iMinVal_px = m_iMinVal_px;
    pDiagScale->m_iMaxVal_px = m_iMaxVal_px;

    for( iLayer = 0; iLayer < CEnumDivLineLayer::count(); iLayer++ )
    {
        pDiagScale->m_ariDivLineDistMin_px[iLayer] = m_ariDivLineDistMin_px[iLayer];
    }

    //pDiagScale->m_uMainDivLineCount = m_uMainDivLineCount;
    //if( pDiagScale->m_uMainDivLineCount > 0 )
    //{
    //    pDiagScale->m_arfMainDivLineVal = new double[pDiagScale->m_uMainDivLineCount];
    //    pDiagScale->m_arfMainDivLinePix = new double[pDiagScale->m_uMainDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScale->m_uMainDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScale->m_arfMainDivLineVal[idxDivLine] = m_arfMainDivLineVal[idxDivLine];
    //        pDiagScale->m_arfMainDivLinePix[idxDivLine] = m_arfMainDivLinePix[idxDivLine];
    //    }
    //}

    //pDiagScale->m_uSubDivLineCount = m_uSubDivLineCount;
    //if( pDiagScale->m_uSubDivLineCount > 0 )
    //{
    //    pDiagScale->m_arfSubDivLineVal = new double[pDiagScale->m_uSubDivLineCount];
    //    pDiagScale->m_arfSubDivLinePix = new double[pDiagScale->m_uSubDivLineCount];

    //    for( idxDivLine = 0; idxDivLine < pDiagScale->m_uSubDivLineCount; idxDivLine++ )
    //    {
    //        pDiagScale->m_arfSubDivLineVal[idxDivLine] = m_arfSubDivLineVal[idxDivLine];
    //        pDiagScale->m_arfSubDivLinePix[idxDivLine] = m_arfSubDivLinePix[idxDivLine];
    //    }
    //}

    i_pDiagramTrg->addDiagScale(pDiagScale);

    return pDiagScale;

} // clone

/*==============================================================================
protected: // type definitions and constants
==============================================================================*/

//------------------------------------------------------------------------------
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
