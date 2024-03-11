/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSDraw/Common/ZSDrawingSize.h"
#include "ZSDraw/Common/ZSDrawAux.h"
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSDraw/Drawing/ZSDrawingScene.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

#if QT_VERSION < 0x050000
#include <QtXml/qxmlstream.h>
#else
#include <QtCore/qxmlstream.h>
#endif

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


/*******************************************************************************
class CDrawingSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates an instance of the class.

    @param i_strName [in] Name of the instance.
*/
CDrawingSize::CDrawingSize(const QString& i_strName) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_eDimensionUnit(EScaleDimensionUnit::Pixels),
    m_metricUnit(Units.Length.mm),
    m_fScreenResolution_px_mm(Units.Length.screenResolutionInPxPerMM()), // reasonable value close to what many monitors are using
    m_iImageMetricCoorsDecimals(2), // to indicate 1.0/4.0 = 0.25
    m_fImageMetricWidth(0.0),
    m_fImageMetricHeight(0.0),
    m_eNormedPaperSize(),
    m_eNormedPaperOrientation(),
    m_iMetricScaleFactorDividend(1),
    m_iMetricScaleFactorDivisor(1),
    m_eYScaleAxisOrientation(EYScaleAxisOrientation::TopDown),
    m_fImageSizeRes_px(1.0),
    m_fImageSizeWidth_px(0.0),
    m_fImageSizeHeight_px(0.0),
    m_pTrcAdminObj(nullptr)
{
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName(), m_strName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

} // ctor

//------------------------------------------------------------------------------
/*! Creates an instance of the class.
*/
CDrawingSize::CDrawingSize(const CDrawingSize& i_other) :
//------------------------------------------------------------------------------
    m_strName(i_other.m_strName),
    m_eDimensionUnit(i_other.m_eDimensionUnit),
    m_metricUnit(i_other.m_metricUnit),
    m_fScreenResolution_px_mm(i_other.m_fScreenResolution_px_mm),
    m_iImageMetricCoorsDecimals(i_other.m_iImageMetricCoorsDecimals),
    m_fImageMetricWidth(i_other.m_fImageMetricWidth),
    m_fImageMetricHeight(i_other.m_fImageMetricHeight),
    m_eNormedPaperSize(i_other.m_eNormedPaperSize),
    m_eNormedPaperOrientation(i_other.m_eNormedPaperOrientation),
    m_iMetricScaleFactorDividend(i_other.m_iMetricScaleFactorDividend),
    m_iMetricScaleFactorDivisor(i_other.m_iMetricScaleFactorDivisor),
    m_eYScaleAxisOrientation(i_other.m_eYScaleAxisOrientation),
    m_fImageSizeRes_px(i_other.m_fImageSizeRes_px),
    m_fImageSizeWidth_px(i_other.m_fImageSizeWidth_px),
    m_fImageSizeHeight_px(i_other.m_fImageSizeHeight_px),
    m_pTrcAdminObj(nullptr)
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_other.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ strMthInArgs );

} // copy ctor

//------------------------------------------------------------------------------
CDrawingSize::~CDrawingSize()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "dtor",
        /* strAddInfo   */ "" );

    mthTracer.onAdminObjAboutToBeReleased();
    CTrcServer::ReleaseTraceAdminObj(m_pTrcAdminObj);

    m_eDimensionUnit = static_cast<EScaleDimensionUnit>(0);
    //m_metricUnit;
    m_fScreenResolution_px_mm = 0.0;
    m_iImageMetricCoorsDecimals = 0;
    m_fImageMetricWidth = 0.0;
    m_fImageMetricHeight = 0.0;
    m_eNormedPaperSize = static_cast<ENormedPaperSize>(0);
    m_eNormedPaperOrientation = static_cast<EOrientation>(0);
    m_iMetricScaleFactorDividend = 0;
    m_iMetricScaleFactorDivisor = 0;
    m_eYScaleAxisOrientation = static_cast<EYScaleAxisOrientation>(0);
    m_fImageSizeRes_px = 0.0;
    m_fImageSizeWidth_px = 0.0;
    m_fImageSizeHeight_px = 0.0;
    m_pTrcAdminObj = nullptr;

} // dtor

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
CDrawingSize& CDrawingSize::operator = (const CDrawingSize& i_other)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_other.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "=Other",
        /* strAddInfo   */ strMthInArgs );

    m_eDimensionUnit = i_other.m_eDimensionUnit;
    m_metricUnit = i_other.m_metricUnit;
    m_fScreenResolution_px_mm = i_other.m_fScreenResolution_px_mm;
    m_iImageMetricCoorsDecimals = i_other.m_iImageMetricCoorsDecimals;
    m_fImageMetricWidth = i_other.m_fImageMetricWidth;
    m_fImageMetricHeight = i_other.m_fImageMetricHeight;
    m_eNormedPaperSize = i_other.m_eNormedPaperSize;
    m_eNormedPaperOrientation = i_other.m_eNormedPaperOrientation;
    m_iMetricScaleFactorDividend = i_other.m_iMetricScaleFactorDividend;
    m_iMetricScaleFactorDivisor = i_other.m_iMetricScaleFactorDivisor;
    m_eYScaleAxisOrientation = i_other.m_eYScaleAxisOrientation;
    m_fImageSizeWidth_px = i_other.m_fImageSizeWidth_px;
    m_fImageSizeWidth_px = i_other.m_fImageSizeWidth_px;
    m_fImageSizeHeight_px = i_other.m_fImageSizeHeight_px;

    return *this;
}

/*==============================================================================
public: // operators
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingSize::operator == ( const CDrawingSize& i_other ) const
//------------------------------------------------------------------------------
{
    bool bEqual = true;
    if( m_eDimensionUnit != i_other.m_eDimensionUnit ) {
        bEqual = false;
    }
    else if( m_metricUnit != i_other.m_metricUnit ) {
        bEqual = false;
    }
    else if( m_fScreenResolution_px_mm != i_other.m_fScreenResolution_px_mm ) {
        bEqual = false;
    }
    else if( m_iImageMetricCoorsDecimals != i_other.m_iImageMetricCoorsDecimals ) {
        bEqual = false;
    }
    else if( m_fImageMetricWidth != i_other.m_fImageMetricWidth ) {
        bEqual = false;
    }
    else if( m_fImageMetricHeight != i_other.m_fImageMetricHeight ) {
        bEqual = false;
    }
    else if( m_eNormedPaperSize != i_other.m_eNormedPaperSize ) {
        bEqual = false;
    }
    else if( m_eNormedPaperOrientation != i_other.m_eNormedPaperOrientation ) {
        bEqual = false;
    }
    else if( m_iMetricScaleFactorDividend != i_other.m_iMetricScaleFactorDividend ) {
        bEqual = false;
    }
    else if( m_iMetricScaleFactorDivisor != i_other.m_iMetricScaleFactorDivisor ) {
        bEqual = false;
    }
    else if( m_eYScaleAxisOrientation != i_other.m_eYScaleAxisOrientation ) {
        bEqual = false;
    }
    else if( m_fImageSizeRes_px != i_other.m_fImageSizeRes_px ) {
        bEqual = false;
    }
    else if( m_fImageSizeWidth_px != i_other.m_fImageSizeWidth_px ) {
        bEqual = false;
    }
    else if( m_fImageSizeHeight_px != i_other.m_fImageSizeHeight_px ) {
        bEqual = false;
    }
    return bEqual;
}

//------------------------------------------------------------------------------
bool CDrawingSize::operator != ( const CDrawingSize& i_other ) const
//------------------------------------------------------------------------------
{
    return !(*this == i_other);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
bool CDrawingSize::isValid() const
//------------------------------------------------------------------------------
{
    bool bIsValid = true;
    if (!m_eDimensionUnit.isValid()) {
        bIsValid = false;
    }
    else if (!m_metricUnit.isValid()) {
        bIsValid = false;
    }
    else if (m_fScreenResolution_px_mm <= 0.0) {
        bIsValid = false;
    }
    else if (m_fImageSizeRes_px <= 0.0) {
        bIsValid = false;
    }
    else if (m_fImageSizeWidth_px <= 0.0 || m_fImageSizeHeight_px <= 0.0) {
        bIsValid = false;
    }
    else if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        if (m_fImageMetricWidth <= 0.0 || m_fImageMetricHeight <= 0.0) {
            bIsValid = false;
        }
        else if (m_iMetricScaleFactorDividend <= 0.0 || m_iMetricScaleFactorDivisor <= 0.0) {
            bIsValid = false;
        }
        else if (!m_eYScaleAxisOrientation.isValid()) {
            bIsValid = false;
        }
    }
    return bIsValid;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::setDimensionUnit( const CEnumScaleDimensionUnit& i_eDimensionUnit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_eDimensionUnit.toString() + " (Prev: " + m_eDimensionUnit.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setDimensionUnit",
        /* strAddInfo   */ strMthInArgs );

    if (m_eDimensionUnit != i_eDimensionUnit) {
        m_eDimensionUnit = i_eDimensionUnit;
        if (isValid()) {
            if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
                CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);
                CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit);
                double fImageMetricWidth_mm = physValWidth.getVal(Units.Length.mm);
                double fImageMetricHeight_mm = physValHeight.getVal(Units.Length.mm);
                double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                                / static_cast<double>(m_iMetricScaleFactorDivisor);
                double fScaledWidth_mm = fFactor * fImageMetricWidth_mm;
                double fScaledHeight_mm = fFactor * fImageMetricHeight_mm;
                m_fImageSizeWidth_px = m_fScreenResolution_px_mm * fScaledWidth_mm;
                m_fImageSizeHeight_px = m_fScreenResolution_px_mm * fScaledHeight_mm;
            }
            else /*if (m_eDimensionUnit == EScaleDimensionUnit::Metric)*/ {
                // Keep the image size in pixels but update the image size in metric unit.
                double fImageMetricWidth_mm = m_fImageSizeWidth_px / m_fScreenResolution_px_mm;
                double fImageMetricHeight_mm = m_fImageSizeHeight_px / m_fScreenResolution_px_mm;
                double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                                / static_cast<double>(m_iMetricScaleFactorDivisor);
                double fScaledWidth_mm = fFactor * fImageMetricWidth_mm;
                double fScaledHeight_mm = fFactor * fImageMetricHeight_mm;
                CPhysVal physValWidth(fScaledWidth_mm, Units.Length.mm);
                CPhysVal physValHeight(fScaledHeight_mm, Units.Length.mm);
                physValWidth.convertValue(m_metricUnit);
                physValHeight.convertValue(m_metricUnit);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                // In order to draw division lines at min and max scale the width
                // in pixels got to be extended by one pixel when using metric scales
                // (see also documentation at class CScaleDivLines).
                m_fImageSizeWidth_px += 1;
                m_fImageSizeHeight_px += 1;
            }
        }
    }
}

//------------------------------------------------------------------------------
CEnumScaleDimensionUnit CDrawingSize::dimensionUnit() const
//------------------------------------------------------------------------------
{
    return m_eDimensionUnit;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current unit to be used.

    If dimension unit is set to Pixels, px is returned.
    If dimension unit is set to Metric, the metric unit is returned.
*/
CUnit CDrawingSize::unit() const
//------------------------------------------------------------------------------
{
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        return Units.Length.px;
    }
    return m_metricUnit;
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of a pixel on the screen in "pixels/mm".

    The screen resolution is also set at the Units.Length so that following
    unit conversion routines between pixels and metric units are using the
    updated conversion factor.

    If the current dimension unit of the drawing size is set to Pixels,
    the image width and height in metrics unit is adjusted.
    If the current dimension unit of the drawing size is set to Metric,
    the image width and height in pixels is adjusted.

    @param [in] i_fRes_px_mm
        Resolution of a screen pixel in "pixels/mm".
*/
void CDrawingSize::setScreenResolutionInPxPerMM(double i_fRes_px_mm)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_fRes_px_mm);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScreenResolutionInPxPerMM",
        /* strAddInfo   */ strMthInArgs );

    // Set the screen resolution also at the Units.Length so that following
    // unit conversion routines between pixels and metric units are using the
    // updated conversion factor.
    m_fScreenResolution_px_mm = i_fRes_px_mm;

    if (isValid()) {
        if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
            // Keep the image size in pixels but update the image size in metric unit.
            updateImageSizeMetrics();
        }
        else /*if (m_eDimensionUnit == EScaleDimensionUnit::Metric)*/ {
            // Keep the image size in metric unit but update the image size in pixels.
            updateImageSizeInPixels();
        }
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of a pixel on the screen in "pixels/<unit>".

    @param [in] i_unit
        Unit in which the value should be returned.
        If the unit is not a metric unit an exception is thrown.
*/
double CDrawingSize::screenResolutionInPxPerMM() const
//------------------------------------------------------------------------------
{
    return m_fScreenResolution_px_mm;
}

//------------------------------------------------------------------------------
/*! @brief Returns the width of a screen pixel in the current unit of the drawing size.

    The width of a screen pixel is defined by the screen resolution in pixels/mm.
*/
CPhysVal CDrawingSize::screenPixelWidth() const
//------------------------------------------------------------------------------
{
    return CPhysVal(1.0/m_fScreenResolution_px_mm, m_metricUnit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the width of a screen pixel in the given unit.

    The width of a screen pixel is defined by the screen resolution in pixels/mm.

    @param [in] i_unit
        Unit in which the value should be returned.
        If the unit is not a metric unit an exception is thrown.
*/
CPhysVal CDrawingSize::screenPixelWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (!Units.Length.isMetricUnit(i_unit)) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::screenPixelWidth(" + i_unit.symbol() + ")");
    }
    CPhysVal physVal(1.0/m_fScreenResolution_px_mm, m_metricUnit);
    physVal.convertValue(i_unit);
    return physVal;
}

//------------------------------------------------------------------------------
/*! @brief Sets the number of decimals to be used for editing and indicating
           coordinates in metric system.

    @param [in] i_iDecimals
        Number of decimals. Must be >= 0.
*/
void CDrawingSize::setMetricImageCoorsDecimals(int i_iDecimals)
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDecimals);
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricImageCoorsDecimals",
        /* strAddInfo   */ strMthInArgs );
    if (i_iDecimals < 0) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setMetricImageCoorsDecimals(" + QString::number(i_iDecimals) + ")");
    }
    m_iImageMetricCoorsDecimals = i_iDecimals;
}

//------------------------------------------------------------------------------
/*! @brief Returns the number of decimals used fro editing and indicating
           coordinates in metric system.
*/
int CDrawingSize::metricImageCoorsDecimals() const
//------------------------------------------------------------------------------
{
    return m_iImageMetricCoorsDecimals;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current resolution in pixels. Usually 1.0.
*/
//------------------------------------------------------------------------------
double CDrawingSize::imageCoorsResolutionInPx() const
{
    return m_fImageSizeRes_px;
}

//------------------------------------------------------------------------------
/*! @brief Returns the current resolution in the current unit of the drawing size.
*/
CPhysValRes CDrawingSize::imageCoorsResolution() const
//------------------------------------------------------------------------------
{
    if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        return CPhysValRes(pow(10.0, -m_iImageMetricCoorsDecimals), m_metricUnit);
    }
    return CPhysValRes(m_fImageSizeRes_px, Units.Length.px);
}

//------------------------------------------------------------------------------
/*! @brief Returns the current resolution.

    @param [in] i_unit
        Unit in which the value should be returned.
        If the desired unit is Pixels, the resolution in pixels is returned.
        If the desired unit is a metric unit, the resolution in metric unit is returned.
*/
CPhysValRes CDrawingSize::imageCoorsResolution(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (Units.Length.isMetricUnit(i_unit)) {
        CPhysValRes physValRes(pow(10.0, -m_iImageMetricCoorsDecimals), m_metricUnit);
        physValRes.convertValue(i_unit);
        return physValRes;
    }
    return CPhysValRes(m_fImageSizeRes_px, Units.Length.px);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::setMetricUnit( const CUnit& i_unit )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_unit.symbol() + " (Prev: " + m_metricUnit.symbol() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setMetricUnit",
        /* strAddInfo   */ strMthInArgs );

    if( m_metricUnit != i_unit ) {
        // If changing the unit the metrics width and height and also the
        // size in pixels remain the same. Only the unit in which the values
        // are indicated will be changed.

        // Current values:
        CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);
        CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit);

        // Convert into new unit:
        m_metricUnit = i_unit;
        physValWidth.convertValue(m_metricUnit);
        physValHeight.convertValue(m_metricUnit);

        // New values. Also the resolution to edit the coors need to be
        // converted to the new unit.
        m_fImageMetricWidth = physValWidth.getVal();
        m_fImageMetricHeight = physValHeight.getVal();
    }
}

//------------------------------------------------------------------------------
CUnit CDrawingSize::metricUnit() const
//------------------------------------------------------------------------------
{
    return m_metricUnit;
}

//------------------------------------------------------------------------------
void CDrawingSize::setNormedPaperSize( const CEnumNormedPaperSize& i_ePaperSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_ePaperSize.toString() + " (Prev: " + m_eNormedPaperSize.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperSize",
        /* strAddInfo   */ strMthInArgs );

    if( m_eNormedPaperSize != i_ePaperSize ) {
        m_eNormedPaperSize = i_ePaperSize;
        if (m_eNormedPaperSize.isValid()) {
            if( !m_eNormedPaperOrientation.isValid() ) {
                m_eNormedPaperOrientation = EOrientation::Horizontal;
            }
            QVariant varPaperSize = m_eNormedPaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( m_eNormedPaperOrientation == EOrientation::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_metricUnit);
                physValHeight.convertValue(m_metricUnit);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
CEnumNormedPaperSize CDrawingSize::normedPaperSize() const
//------------------------------------------------------------------------------
{
    return m_eNormedPaperSize;
}

//------------------------------------------------------------------------------
void CDrawingSize::setNormedPaperOrientation( const CEnumOrientation& i_orientation )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_orientation.toString() + " (Prev: " + m_eNormedPaperOrientation.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setNormedPaperOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_eNormedPaperOrientation != i_orientation ) {
        m_eNormedPaperOrientation = i_orientation;
        // Setting values according to orientation makes only sense for normed paper sizes.
        // For user defined paper sizes there is no orientation. The width might be
        // greater than the height or vice versa. The orientation will be stored if later
        // on a normed paper size is set but for now the values will not be adjusted.
        if (m_eNormedPaperSize.isValid()) {
            QVariant varPaperSize = m_eNormedPaperSize.toValue();
            if( varPaperSize.type() == QVariant::SizeF ) {
                QSizeF sizeF = varPaperSize.toSizeF();
                // Default: Horizontal Orientation
                CPhysVal physValWidth(sizeF.height(), Units.Length.mm);
                CPhysVal physValHeight(sizeF.width(), Units.Length.mm);
                if( m_eNormedPaperOrientation == EOrientation::Vertical ) {
                    physValWidth = sizeF.width();
                    physValHeight = sizeF.height();
                }
                physValWidth.convertValue(m_metricUnit);
                physValHeight.convertValue(m_metricUnit);
                m_fImageMetricWidth = physValWidth.getVal();
                m_fImageMetricHeight = physValHeight.getVal();
                updateImageSizeInPixels();
            }
        }
    }
}

//------------------------------------------------------------------------------
CEnumOrientation CDrawingSize::normedPaperOrientation() const
//------------------------------------------------------------------------------
{
    return m_eNormedPaperOrientation;
}

//------------------------------------------------------------------------------
void CDrawingSize::setScaleFactor( int i_iDividend, int i_iDivisor )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = QString::number(i_iDividend) + "/" + QString::number(i_iDivisor)
                     + " (Prev: " + QString::number(m_iMetricScaleFactorDividend)
                     + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setScaleFactor",
        /* strAddInfo   */ strMthInArgs );

    if( i_iDividend <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Dividend <= 0!");
    }
    if( i_iDivisor <= 0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setScaleFactor("
                + QString::number(i_iDividend) + ", " + QString::number(i_iDivisor) + "): "
                + "!Divisor <= 0!");
    }
    if( m_iMetricScaleFactorDividend != i_iDividend || m_iMetricScaleFactorDivisor != i_iDivisor ) {
        m_iMetricScaleFactorDividend = i_iDividend;
        m_iMetricScaleFactorDivisor = i_iDivisor;
        updateImageSizeInPixels();
    }
}

//------------------------------------------------------------------------------
int CDrawingSize::scaleFactorDividend() const
//------------------------------------------------------------------------------
{
    return m_iMetricScaleFactorDividend;
}

//------------------------------------------------------------------------------
int CDrawingSize::scaleFactorDivisor() const
//------------------------------------------------------------------------------
{
    return m_iMetricScaleFactorDivisor;
}

//------------------------------------------------------------------------------
void CDrawingSize::setYScaleAxisOrientation( const CEnumYScaleAxisOrientation& i_orientation )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_orientation.toString() + " (Prev: " + m_eYScaleAxisOrientation.toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setYScaleAxisOrientation",
        /* strAddInfo   */ strMthInArgs );

    if( m_eYScaleAxisOrientation != i_orientation ) {
        m_eYScaleAxisOrientation = i_orientation;
    }
}

//------------------------------------------------------------------------------
CEnumYScaleAxisOrientation CDrawingSize::yScaleAxisOrientation() const
//------------------------------------------------------------------------------
{
    return m_eYScaleAxisOrientation;
}

//------------------------------------------------------------------------------
void CDrawingSize::setImageSize( const CPhysValSize& i_physValSize )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = "{" + i_physValSize.toString() + "} " + i_physValSize.unit().symbol();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setImageSize",
        /* strAddInfo   */ strMthInArgs );

    setImageSize(i_physValSize.width(), i_physValSize.height());
}

//------------------------------------------------------------------------------
void CDrawingSize::setImageSize( const CPhysVal& i_physValWidth, const CPhysVal& i_physValHeight )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValWidth.toString() + ", " + i_physValHeight.toString()
                    + " (Prev: " + CPhysVal(m_fImageSizeWidth_px, Units.Length.px).toString()
                    + ", " + CPhysVal(m_fImageSizeHeight_px, Units.Length.px).toString() + ")"
                    + ", (Prev: " + CPhysVal(m_fImageMetricWidth, m_metricUnit).toString()
                    + ", " + CPhysVal(m_fImageMetricHeight, m_metricUnit).toString() + ")";
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setImageSize",
        /* strAddInfo   */ strMthInArgs );

    if( !i_physValWidth.isValid() || i_physValWidth.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValWidth <= 0.0!");
    }
    if( !i_physValHeight.isValid() || i_physValHeight.getVal() <= 0.0 ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "!i_physValHeight <= 0.0!");
    }
    if( (i_physValWidth.unit() == Units.Length.px && i_physValHeight.unit() != Units.Length.px)
     || (i_physValWidth.unit() != Units.Length.px && i_physValHeight.unit() == Units.Length.px) ) {
        throw CException(
            __FILE__, __LINE__, EResultArgOutOfRange,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Either set size in pixels or in metrics");
    }

    if( i_physValWidth.unit() == Units.Length.px ) {
        if (m_eDimensionUnit != EScaleDimensionUnit::Pixels) {
            throw CException(__FILE__, __LINE__, EResultInvalidMethodCall,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Setting the drawing size in pixels only allowed for pixel drawings");
        }
        double fImageSizeWidth_px = i_physValWidth.getVal();
        double fImageSizeHeight_px = i_physValHeight.getVal();
        if( m_fImageSizeWidth_px != fImageSizeWidth_px || m_fImageSizeHeight_px != fImageSizeHeight_px ) {
            m_fImageSizeWidth_px = fImageSizeWidth_px;
            m_fImageSizeHeight_px = fImageSizeHeight_px;
            updateImageSizeMetrics();
        }
    }
    else {
        if (m_eDimensionUnit != EScaleDimensionUnit::Metric) {
            throw CException(__FILE__, __LINE__, EResultInvalidMethodCall,
            ClassName() + "::setImageSize("
                + i_physValWidth.toString() + ", " + i_physValHeight.toString() + "): "
                + "Setting the drawing size in metric unit only allowed for metric drawings");
        }
        double fImageMetricWidth = i_physValWidth.getVal(m_metricUnit);
        double fImageMetricHeight = i_physValHeight.getVal(m_metricUnit);
        if( m_fImageMetricWidth != fImageMetricWidth || m_fImageMetricHeight != fImageMetricHeight ) {
            m_fImageMetricWidth = fImageMetricWidth;
            m_fImageMetricHeight = fImageMetricHeight;
            updateImageSizeInPixels();
            updatePaperFormat();
        }
    }
}

//------------------------------------------------------------------------------
QSize CDrawingSize::imageSizeInPixels() const
//------------------------------------------------------------------------------
{
    return QSize(m_fImageSizeWidth_px, m_fImageSizeHeight_px);
}

//------------------------------------------------------------------------------
int CDrawingSize::imageWidthInPixels() const
//------------------------------------------------------------------------------
{
    return m_fImageSizeWidth_px;
}

//------------------------------------------------------------------------------
int CDrawingSize::imageHeightInPixels() const
//------------------------------------------------------------------------------
{
    return m_fImageSizeHeight_px;
}

//------------------------------------------------------------------------------
CPhysValSize CDrawingSize::metricImageSize() const
//------------------------------------------------------------------------------
{
    double fRes = imageCoorsResolution(m_metricUnit).getVal();
    return CPhysValSize(m_fImageMetricWidth, m_fImageMetricHeight, fRes, m_metricUnit);
}

//------------------------------------------------------------------------------
CPhysValSize CDrawingSize::metricImageSize(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (Units.Length.isMetricUnit(i_unit)) {
        CPhysVal physValWidth = metricImageWidth(i_unit);
        CPhysVal physValHeight = metricImageHeight(i_unit);
        return CPhysValSize(physValWidth, physValHeight);
    }
    return CPhysValSize(m_fImageSizeWidth_px, m_fImageSizeHeight_px, m_fImageSizeRes_px, Units.Length.px);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageWidth() const
//------------------------------------------------------------------------------
{
    double fRes = imageCoorsResolution(m_metricUnit).getVal();
    return CPhysVal(m_fImageMetricWidth, m_metricUnit, fRes);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (Units.Length.isMetricUnit(i_unit)) {
        double fRes = imageCoorsResolution(m_metricUnit).getVal();
        CPhysVal physVal(m_fImageMetricWidth, m_metricUnit, fRes);
        physVal.convertValue(i_unit);
        return physVal;
    }
    return CPhysVal(m_fImageSizeWidth_px, Units.Length.px, m_fImageSizeRes_px);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageHeight() const
//------------------------------------------------------------------------------
{
    double fRes = imageCoorsResolution(m_metricUnit).getVal();
    return CPhysVal(m_fImageMetricHeight, m_metricUnit, fRes);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageHeight(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    if (Units.Length.isMetricUnit(i_unit)) {
        double fRes = imageCoorsResolution(m_metricUnit).getVal();
        CPhysVal physVal(m_fImageMetricHeight, m_metricUnit, fRes);
        physVal.convertValue(i_unit);
        return physVal;
    }
    return CPhysVal(m_fImageSizeHeight_px, Units.Length.px, m_fImageSizeRes_px);
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::save( QXmlStreamWriter& i_xmlStreamWriter )
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "save",
        /* strAddInfo   */ "" );

    i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrDimensionUnit, dimensionUnit().toString());
    if (dimensionUnit() == EScaleDimensionUnit::Pixels) {
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrWidth, QString::number(imageSizeInPixels().width()));
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrHeight, QString::number(imageSizeInPixels().height()));
    }
    else /*if (m_drawingSize.dimensionUnit() == EScaleDimensionUnit::Metric)*/ {
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrScreenResolutionPxPerMilliMeter, QString::number(screenResolutionInPxPerMM()));
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrMetricImageCoorsDecimals, QString::number(metricImageCoorsDecimals()));
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrUnit, metricUnit().symbol());
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrWidth, metricImageWidth().toString());
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrHeight, metricImageHeight().toString());
        i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrScaleFactor,
            QString::number(scaleFactorDividend()) + ":" + QString::number(scaleFactorDivisor()));
        if (normedPaperSize().isValid()) {
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrPaperSize, normedPaperSize().toString());
        }
        if (normedPaperOrientation().isValid()) {
            i_xmlStreamWriter.writeAttribute(XmlStreamParser::c_strXmlAttrPaperOrientation, normedPaperOrientation().toString());
        }
    }
} // save

//------------------------------------------------------------------------------
void CDrawingSize::load( QXmlStreamReader& i_xmlStreamReader )
//------------------------------------------------------------------------------
{
    QString strElemName = i_xmlStreamReader.name().toString();
    QXmlStreamAttributes xmlStreamAttrs = i_xmlStreamReader.attributes();
    CEnumScaleDimensionUnit dimensionUnit = XmlStreamParser::getDimensionUnit(
        i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrDimensionUnit);
    if (!i_xmlStreamReader.hasError()) {
        setDimensionUnit(dimensionUnit);
        if (dimensionUnit == EScaleDimensionUnit::Pixels) {
            double cxWidth_px = XmlStreamParser::getDoubleVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrWidth);
            double cyHeight_px = XmlStreamParser::getDoubleVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrHeight);
            if (!i_xmlStreamReader.hasError()) {
                setImageSize(CPhysVal(cxWidth_px, Units.Length.px), CPhysVal(cyHeight_px, Units.Length.px));
            }
        }
        else if (dimensionUnit == EScaleDimensionUnit::Metric) {
            double fScreenResPxPerMM = XmlStreamParser::getDoubleVal(
                i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrScreenResolutionPxPerMilliMeter);
            if (!i_xmlStreamReader.hasError()) {
                setScreenResolutionInPxPerMM(fScreenResPxPerMM);
            }
            if (!i_xmlStreamReader.hasError()) {
                int iDecimals = XmlStreamParser::getIntVal(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrMetricImageCoorsDecimals);
                if (!i_xmlStreamReader.hasError()) {
                    setMetricImageCoorsDecimals(iDecimals);
                }
            }
            if (!i_xmlStreamReader.hasError()) {
                CUnit unit = XmlStreamParser::getUnit(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrUnit);
                setMetricUnit(unit);
            }
            if (!i_xmlStreamReader.hasError()) {
                CPhysVal physValWidth = XmlStreamParser::getPhysVal(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrWidth);
                CPhysVal physValHeight = XmlStreamParser::getPhysVal(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrHeight);
                if (!i_xmlStreamReader.hasError()) {
                    setImageSize(physValWidth, physValHeight);
                }
            }
            if (!i_xmlStreamReader.hasError()) {
                std::pair<int, int> scaleFactor = XmlStreamParser::getIntPair(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrScaleFactor);
                if (!i_xmlStreamReader.hasError()) {
                    setScaleFactor(scaleFactor.first, scaleFactor.second);
                }
            }
            if (!i_xmlStreamReader.hasError()) {
                CEnumNormedPaperSize paperSize = XmlStreamParser::getNormedPaperSize(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrPaperSize, false);
                if (!i_xmlStreamReader.hasError() && paperSize.isValid()) {
                    setNormedPaperSize(paperSize);
                }
            }
            if (!i_xmlStreamReader.hasError()) {
                CEnumOrientation orientation = XmlStreamParser::getOrientation(
                    i_xmlStreamReader, xmlStreamAttrs, strElemName, XmlStreamParser::c_strXmlAttrPaperOrientation, false);
                if (!i_xmlStreamReader.hasError() && orientation.isValid()) {
                    setNormedPaperOrientation(orientation);
                }
            }
        }
    }
} // load

/*==============================================================================
protected: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Updates the image size in pixels.

    In order to draw division lines at min and max scale the width
    in pixels got to be extended by one pixel when using metric scales
    (see also documentation at class CScaleDivLines).

    This must be taken into account by the CDrawingSize class when calculating
    the width and height of the image size in pixels.
*/
void CDrawingSize::updateImageSizeInPixels()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeInPixels",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_eDimensionUnit != EScaleDimensionUnit::Metric) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);
    CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit);
    double fImageMetricWidth_mm = physValWidth.getVal(Units.Length.mm);
    double fImageMetricHeight_mm = physValHeight.getVal(Units.Length.mm);

    //// In order to draw division lines at min and max scale the width
    //// in pixels got to be extended by one pixel when using metric scales
    //// (see also documentation at class CScaleDivLines).
    //if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
    //    fImageMetricWidth_mm += 1.0 / m_fScreenResolution_px_mm;
    //    fImageMetricHeight_mm += 1.0 / m_fScreenResolution_px_mm;
    //}

    double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                    / static_cast<double>(m_iMetricScaleFactorDivisor);
    double fScaledWidth_mm = fFactor * fImageMetricWidth_mm;
    double fScaledHeight_mm = fFactor * fImageMetricHeight_mm;
    m_fImageSizeWidth_px = m_fScreenResolution_px_mm * fScaledWidth_mm;
    m_fImageSizeHeight_px = m_fScreenResolution_px_mm * fScaledHeight_mm;
    // In order to draw division lines at min and max scale the width
    // in pixels got to be extended by one pixel when using metric scales
    // (see also documentation at class CScaleDivLines).
    if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        m_fImageSizeWidth_px += 1;
        m_fImageSizeHeight_px += 1;
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeInPixels

//------------------------------------------------------------------------------
/*! @brief Updates the image size in metric unit.

    In order to draw division lines at min and max scale the width
    in pixels got to be extended by one pixel when using metric scales
    (see also documentation at class CScaleDivLines).

    This must be taken into account by the CDrawingSize class when calculating
    the width and height of the image size in pixels.
*/
void CDrawingSize::updateImageSizeMetrics()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updateImageSizeMetrics",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }
    if (m_eDimensionUnit != EScaleDimensionUnit::Pixels) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    double fImageSizeWidth_px = m_fImageSizeWidth_px;
    double fImageSizeHeight_px = m_fImageSizeHeight_px;
    // In order to draw division lines at min and max scale the width
    // in pixels got to be extended by one pixel when using metric scales
    // (see also documentation at class CScaleDivLines).
    //if (isValid() && m_eDimensionUnit == EScaleDimensionUnit::Metric) {
    //    fImageSizeWidth_px -= 1;
    //    fImageSizeHeight_px -= 1;
    //}
    double fImageMetricWidth_mm = fImageSizeWidth_px / m_fScreenResolution_px_mm;
    double fImageMetricHeight_mm = fImageSizeHeight_px / m_fScreenResolution_px_mm;
    //// In order to draw division lines at min and max scale the width
    //// in pixels got to be extended by one pixel when using metric scales
    //// (see also documentation at class CScaleDivLines).
    //if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
    //    fImageMetricWidth_mm -= 1.0 / m_fScreenResolution_px_mm;
    //    fImageMetricHeight_mm -= 1.0 / m_fScreenResolution_px_mm;
    //}
    CPhysVal physValWidth(fImageMetricWidth_mm, Units.Length.mm);
    CPhysVal physValHeight(fImageMetricHeight_mm, Units.Length.mm);
    physValWidth.convertValue(m_metricUnit);
    physValHeight.convertValue(m_metricUnit);
    m_fImageMetricWidth = physValWidth.getVal();
    m_fImageMetricHeight = physValHeight.getVal();

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updateImageSizeMetrics

//------------------------------------------------------------------------------
void CDrawingSize::updatePaperFormat()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "updatePaperFormat",
        /* strAddInfo   */ "" );
    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Enter);
    }

    if (m_eDimensionUnit != EScaleDimensionUnit::Metric) {
        throw CException(__FILE__, __LINE__, EResultInvalidMethodCall);
    }

    bool bNormedPaperSizeFound = false;
    CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit);
    CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit);
    physValWidth.convertValue(Units.Length.mm);
    physValHeight.convertValue(Units.Length.mm);
    QSizeF sizePaper(physValWidth.getVal(), physValHeight.getVal());
    m_eNormedPaperSize = CEnumNormedPaperSize::fromValue(sizePaper, &bNormedPaperSizeFound);
    // If found the orientation is vertical.
    if( bNormedPaperSizeFound ) {
        m_eNormedPaperOrientation = EOrientation::Vertical;
    }
    // If not found try also the other orientation:
    else {
        physValWidth.setVal(m_fImageMetricHeight);
        physValHeight.setVal(m_fImageMetricWidth);
        sizePaper.setWidth(physValWidth.getVal());
        sizePaper.setHeight(physValHeight.getVal());
        m_eNormedPaperSize = CEnumNormedPaperSize::fromValue(sizePaper, &bNormedPaperSizeFound);
        if( bNormedPaperSizeFound ) {
            m_eNormedPaperOrientation = EOrientation::Horizontal;
        }
        else {
            m_eNormedPaperSize = CEnumNormedPaperSize();
            m_eNormedPaperOrientation = CEnumOrientation();
        }
    }

    if( mthTracer.isRuntimeInfoActive(ELogDetailLevel::Debug) ) {
        traceValues(mthTracer, EMethodDir::Leave);
    }
} // updatePaperFormat

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
QString CDrawingSize::toString() const
//------------------------------------------------------------------------------
{
    QString str = m_strName +
        ", IsValid: " + bool2Str(isValid()) +
        ", DimensionUnit: "+ QString(m_eDimensionUnit.isValid() ? m_eDimensionUnit.toString() : "?") +
        ", YScale: " + QString(m_eYScaleAxisOrientation.isValid() ? m_eYScaleAxisOrientation.toString() : "?") +
        ", ScreenResolution: " + QString::number(m_fScreenResolution_px_mm, 'f', 1) + " px/mm" +
        ", Size/" + m_metricUnit.symbol() + " {" + CPhysValSize(m_fImageMetricWidth, m_fImageMetricHeight, imageCoorsResolution(m_metricUnit).getVal(), m_metricUnit).toString() + "}"
        ", Size/px {" + CPhysValSize(m_fImageSizeWidth_px, m_fImageSizeHeight_px, m_fImageSizeRes_px, Units.Length.px).toString() + "}" +
        ", Scale: " + QString::number(m_iMetricScaleFactorDividend) + ":" + QString::number(m_iMetricScaleFactorDivisor) +
        ", PaperSize: " + QString(m_eNormedPaperSize.isValid() ? m_eNormedPaperSize.toString() : "?") +
        ", Orientation: " + QString(m_eNormedPaperOrientation.isValid() ? m_eNormedPaperOrientation.toString() : "?");
    return str;
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ") + toString();
    mthTracer.trace(strMthLog);
}
