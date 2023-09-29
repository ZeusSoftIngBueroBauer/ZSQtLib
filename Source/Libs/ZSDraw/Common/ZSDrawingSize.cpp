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
#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSSys/ZSSysAux.h"
#include "ZSSys/ZSSysTrcMethod.h"
#include "ZSSys/ZSSysTrcServer.h"

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
    m_fImageMetricRes(0.1),
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
CDrawingSize::CDrawingSize(const QString& i_strName, const CDrawingSize& i_other) :
//------------------------------------------------------------------------------
    m_strName(i_strName),
    m_eDimensionUnit(i_other.m_eDimensionUnit),
    m_metricUnit(i_other.m_metricUnit),
    m_fImageMetricRes(i_other.m_fImageMetricRes),
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
    m_pTrcAdminObj = CTrcServer::GetTraceAdminObj(
        NameSpace() + "::Drawing", ClassName(), m_strName);

    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "ctor",
        /* strAddInfo   */ "" );

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
    m_fImageMetricRes = 0.0;
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
    m_fImageMetricRes = i_other.m_fImageMetricRes;
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
    else if( m_fImageMetricRes != i_other.m_fImageMetricRes ) {
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
/*! @brief Called by the drawing scene if the screen resolution in
           pixels/mm has been changed to update the metrics image size.
*/
void CDrawingSize::onDrawUnitsLengthResolutionChanged()
//------------------------------------------------------------------------------
{
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "onDrawUnitsLengthResolutionChanged",
        /* strAddInfo   */ "" );

    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        // Keep the image size in pixels but update the image size in metric unit.
        updateImageSizeMetrics();
    }
    else /*if (m_eDimensionUnit == EScaleDimensionUnit::Metric)*/ {
        // Keep the image size in metric unit but update the image size in pixels.
        updateImageSizeInPixels();
    }
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

    m_eDimensionUnit = i_eDimensionUnit;
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

//------------------------------------------------------------------------------
/*! @brief Sets the current resolution.

    If dimension unit is set to Metric, the metrics width and height is adjusted.
    As a precondition the resolution in pixels/mm must have been set before
    at the Units.Length.
*/
void CDrawingSize::setResolution( const CPhysValRes& i_physValRes )
//------------------------------------------------------------------------------
{
    QString strMthInArgs;
    if (areMethodCallsActive(m_pTrcAdminObj, EMethodTraceDetailLevel::ArgsNormal)) {
        strMthInArgs = i_physValRes.toString();
    }
    CMethodTracer mthTracer(
        /* pAdminObj    */ m_pTrcAdminObj,
        /* iDetailLevel */ EMethodTraceDetailLevel::EnterLeave,
        /* strMethod    */ "setResolution",
        /* strAddInfo   */ strMthInArgs );

    if (i_physValRes.unit() == Units.Length.px) {
        m_fImageSizeRes_px = i_physValRes.getVal();
    }
    else {
        m_fImageMetricRes = i_physValRes.getVal(m_metricUnit);
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the current resolution.

    If dimension unit is set to Pixels, the resolution in pixels is returned.
    If dimension unit is set to Metric, the resolution in metric unit is returned.
*/
CPhysValRes CDrawingSize::resolution() const
//------------------------------------------------------------------------------
{
    if (m_eDimensionUnit == EScaleDimensionUnit::Pixels) {
        return CPhysValRes(m_fImageSizeRes_px, Units.Length.px);
    }
    return CPhysValRes(m_fImageMetricRes, m_metricUnit);
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
        CPhysVal physValWidth(m_fImageMetricWidth, m_metricUnit, m_fImageMetricRes);
        CPhysVal physValHeight(m_fImageMetricHeight, m_metricUnit, m_fImageMetricRes);

        // Convert into new unit:
        m_metricUnit = i_unit;
        physValWidth.convertValue(m_metricUnit);
        physValHeight.convertValue(m_metricUnit);

        // New values. Also the resolution is returned in the new unit.
        m_fImageMetricRes = physValWidth.getRes().getVal();
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
        double fImageSizeWidth_px = i_physValWidth.getVal();
        double fImageSizeHeight_px = i_physValHeight.getVal();
        if( m_fImageSizeWidth_px != fImageSizeWidth_px || m_fImageSizeHeight_px != fImageSizeHeight_px ) {
            m_fImageSizeWidth_px = fImageSizeWidth_px;
            m_fImageSizeHeight_px = fImageSizeHeight_px;
            updateImageSizeMetrics();
            updatePaperFormat();
        }
    }
    else {
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
CPhysVal CDrawingSize::metricImageWidth() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fImageMetricWidth, m_metricUnit, m_fImageMetricRes);
}

//------------------------------------------------------------------------------
CPhysVal CDrawingSize::metricImageHeight() const
//------------------------------------------------------------------------------
{
    return CPhysVal(m_fImageMetricHeight, m_metricUnit, m_fImageMetricRes);
}

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

    double fFactor =  static_cast<double>(m_iMetricScaleFactorDividend)
                    / static_cast<double>(m_iMetricScaleFactorDivisor);
    double fScaledWidth = fFactor * m_fImageMetricWidth;
    double fScaledHeight = fFactor * m_fImageMetricHeight;
    CPhysVal physValWidth(fScaledWidth, m_metricUnit);
    CPhysVal physValHeight(fScaledHeight, m_metricUnit);

    // The unit conversion will take the screen resolution in pixels/mm into account.
    physValWidth.convertValue(Units.Length.px);
    physValHeight.convertValue(Units.Length.px);

    m_fImageSizeWidth_px = physValWidth.getVal();
    m_fImageSizeHeight_px = physValHeight.getVal();

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

    double fImageSizeWidth_px = m_fImageSizeWidth_px;
    double fImageSizeHeight_px = m_fImageSizeHeight_px;

    // In order to draw division lines at min and max scale the width
    // in pixels got to be extended by one pixel when using metric scales
    // (see also documentation at class CScaleDivLines).
    if (m_eDimensionUnit == EScaleDimensionUnit::Metric) {
        fImageSizeWidth_px -= 1;
        fImageSizeHeight_px -= 1;
    }

    CPhysVal physValWidth(fImageSizeWidth_px, Units.Length.px);
    CPhysVal physValHeight(fImageSizeHeight_px, Units.Length.px);

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
    QString str = m_eDimensionUnit.toString() +
        ", NormedPaperSize: " + QString(m_eNormedPaperSize.isValid() ? m_eNormedPaperSize.toString() : "Invalid") +
        ", Orientation: " + QString(m_eNormedPaperOrientation.isValid() ? m_eNormedPaperOrientation.toString() : "Invalid") +
        ", Scale (" + QString::number(m_iMetricScaleFactorDividend) +
            "/" + QString::number(m_iMetricScaleFactorDivisor) + ")" +
        ", Size (" + CPhysVal(m_fImageMetricWidth, m_metricUnit, m_fImageMetricRes).toString() +
            " * " + CPhysVal(m_fImageMetricHeight, m_metricUnit, m_fImageMetricRes).toString() + ")" +
        ", Size (" + CPhysVal(m_fImageSizeWidth_px, Units.Length.px, m_fImageSizeRes_px).toString() +
            " * " + CPhysVal(m_fImageSizeHeight_px, Units.Length.px, m_fImageSizeRes_px).toString() + ")";
    return str;
}

/*==============================================================================
protected: // instance methods (method tracing)
==============================================================================*/

//------------------------------------------------------------------------------
void CDrawingSize::traceValues(CMethodTracer& mthTracer, EMethodDir i_methodDir)
//------------------------------------------------------------------------------
{
    QString strMthLog = QString(i_methodDir == EMethodDir::Enter ? "-+ " : "+- ") +
        m_eDimensionUnit.toString() +
        ", NormedPaperSize: " + QString(m_eNormedPaperSize.isValid() ? m_eNormedPaperSize.toString() : "---") +
        ", NormedOrientation: " + QString(m_eNormedPaperOrientation.isValid() ? m_eNormedPaperOrientation.toString() : "---") +
        ", Scale (" + QString::number(m_iMetricScaleFactorDividend) +
            + "/" + QString::number(m_iMetricScaleFactorDivisor) + ")" +
        ", Size (" + CPhysVal(m_fImageMetricWidth, m_metricUnit, m_fImageMetricRes).toString() +
            " * " + CPhysVal(m_fImageMetricHeight, m_metricUnit, m_fImageMetricRes).toString() + ")" +
        ", Size (" + CPhysVal(m_fImageSizeWidth_px, Units.Length.px, m_fImageSizeRes_px).toString() +
            " * " + CPhysVal(m_fImageSizeHeight_px, Units.Length.px, m_fImageSizeRes_px).toString() + ")";
    mthTracer.trace(strMthLog);
}
