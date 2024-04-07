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

#include "ZSDraw/Common/ZSDrawUnits.h"
#include "ZSSys/ZSSysMath.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Draw;
using namespace ZS::PhysVal;


CUnits ZS::Draw::Units;


/*******************************************************************************
class CUnitsLength : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates the physical size for units in metrics system.

    @param [in] i_pParentBranch
        Parent branch in the index tree for this physical size.
*/
CUnitsLength::CUnitsLength(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* scienceField     */ i_pParentBranch,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "L",
        /* bIsPowerRelated  */ false ),
    m_treeEntryMicroMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_treeEntryCentiMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixCenti ),
    m_treeEntryDeziMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixDezi ),
    m_treeEntryMeter(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKiloMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_treeEntryInch(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Inch",
        /* strSymbol      */ "in",
        /* fMFromSI       */ 1.0/0.0254 ),
    m_treeEntryPixel(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Pixel",
        /* strSymbol      */ "px" ),
    m_treeEntryDots(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Dots",
        /* strSymbol      */ "dots" ),
    um(m_treeEntryMicroMeter),
    mm(m_treeEntryMilliMeter),
    cm(m_treeEntryCentiMeter),
    dm(m_treeEntryDeziMeter),
    m(m_treeEntryMeter),
    km(m_treeEntryKiloMeter),
    in(m_treeEntryInch),
    px(m_treeEntryPixel),
    dots(m_treeEntryDots)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to the chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    m_treeEntryMicroMeter.setNextLowerHigherUnits(nullptr, &m_treeEntryMilliMeter);
    m_treeEntryMilliMeter.setNextLowerHigherUnits(&m_treeEntryMicroMeter, &m_treeEntryMeter);
    m_treeEntryMeter.setNextLowerHigherUnits(&m_treeEntryMilliMeter, &m_treeEntryKiloMeter);
    m_treeEntryKiloMeter.setNextLowerHigherUnits(&m_treeEntryMeter, nullptr);

    //m_treeEntryPixel.setFactorConvertFromSIUnit();

    // To allow "short" unit strings like "mm" we add shortcuts to each unit.
    m_pTree->addShortcut(&m_treeEntryMicroMeter, "um");
    m_pTree->addShortcut(&m_treeEntryMilliMeter, "mm");
    m_pTree->addShortcut(&m_treeEntryCentiMeter, "cm");
    m_pTree->addShortcut(&m_treeEntryDeziMeter, "dm");
    m_pTree->addShortcut(&m_treeEntryMeter, "m");
    m_pTree->addShortcut(&m_treeEntryKiloMeter, "km");
    m_pTree->addShortcut(&m_treeEntryInch, "in");
    m_pTree->addShortcut(&m_treeEntryPixel, "px");
    m_pTree->addShortcut(&m_treeEntryDots, "dots");

    // reasonable value close to what many monitors are using
    setScreenResolutionInPxPerMM(4.0);

    // If not explicitly set, the scale factors default value is used which is 1.0 (1/1).
    setScaleFactor(1, 1);

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the physical size.
*/
CUnitsLength::~CUnitsLength()
//------------------------------------------------------------------------------
{
} // dtor


/*==============================================================================
public: // instance methods (resolution of monitor, pixels per inches)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the logical resolution of a pixel on the screen as reference value
           at the unit group with the name "PixelResolution_mm".

    Different resolutions for horizontal and vertical directions are not supported.
    Anyway very often several monitors are used whose resolution may be different
    both horizontally and vertically.

    @param [in] i_fRes_px_mm
        Resolution of a pixel on the screen pixel in pixels/mm.
*/
void CUnitsLength::setScreenResolutionInPxPerMM( double i_fRes_px_mm )
//------------------------------------------------------------------------------
{
    double fResPrev_px_mm = 0.0;

    if (hasReferenceValue("ScreenResolutionPxPerMM")) {
        fResPrev_px_mm = screenResolutionInPxPerMM();
    }

    if (fResPrev_px_mm != i_fRes_px_mm) {

        setReferenceValue("ScreenResolutionPxPerMM", CPhysVal(i_fRes_px_mm, Units.Length.mm));

        double fipm = m_treeEntryMilliMeter.getFactorConvertFromSIUnit();

        double fScaleFactor = 1.0;
        if (hasReferenceValue("ScaleFactorDividend") && hasReferenceValue("ScaleFactorDivisor")) {
            fScaleFactor = scaleFactor();
        }

        double fpxm = fipm * i_fRes_px_mm * fScaleFactor;

        SFctConvert fctConvertPxFromMeter(
            /* fctConvertType */ EFctConvert_mMULxADDt,
            /* pPhysUnitSrc   */ &m_treeEntryMeter,
            /* pPhysUnitDst   */ &m_treeEntryPixel,
            /* physValM       */ fpxm );

        m_treeEntryPixel.setFctConvertFromSIUnit(fctConvertPxFromMeter);

        emit screenResolutionInPxPerMMChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the screen resolution of a pixel on the screen in pixels/mm.
*/
double CUnitsLength::screenResolutionInPxPerMM() const
//------------------------------------------------------------------------------
{
    return getReferenceValue("ScreenResolutionPxPerMM").getVal();
}

//------------------------------------------------------------------------------
/*! @brief Returns the width of a pixel on the screen in the given unit.

    @param [in] i_unit (default mm)
        Unit in which the width of a pixel has to be returned.
        Please note that it does not make sense to pass pixels or dots as the
        desired unit as the screen resolution or the device resolution in pixels
        or dots is always 1.0.

    @return Width of a pixel on the screen in the given unit.
*/
CPhysVal CUnitsLength::screenPixelWidth(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    double fPxWidth = 1.0;
    if (i_unit == px || i_unit == dots) {
        fPxWidth = 1.0;
    }
    else {
        fPxWidth = 1.0 / getReferenceValue("ScreenResolutionPxPerMM").getVal(i_unit);
    }
    return CPhysVal(fPxWidth, i_unit);
}

/*==============================================================================
public: // instance methods (scale factor of drawing in metric dimension)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the scale factor to be used by the drawing as reference value
           with the names "ScaleFactorDividend" and "ScaleFactorDivisor" to
           the unit group.

    ScaleFactor = Dividend/Divisor

    @Note If not explicitly set the scale factors default value is used which is 1.0 (1/1).

    @param [in] i_iDividend
        Dividend of the scale factor.
    @param [in] i_iDivisor
        Divisor of the scale factor.
*/
void CUnitsLength::setScaleFactor(int i_iDividend, int i_iDivisor)
//------------------------------------------------------------------------------
{
    int iDividendPrev = 1;
    int iDivisorPrev = 1;

    if (hasReferenceValue("ScaleFactorDividend") && hasReferenceValue("ScaleFactorDivisor")) {
        iDividendPrev = scaleFactorDividend();
        iDivisorPrev = scaleFactorDivisor();
    }

    if (iDividendPrev != i_iDividend || iDivisorPrev != i_iDivisor) {

        setReferenceValue("ScaleFactorDividend", i_iDividend);
        setReferenceValue("ScaleFactorDivisor", i_iDivisor);

        double fFac = m_treeEntryMilliMeter.getFactorConvertFromSIUnit();

        double fpxpmm = 1.0;
        if (hasReferenceValue("PixelResolution_mm")) {
            fpxpmm = 1.0/screenPixelWidth(mm).getVal();
        }

        fFac = fFac * fpxpmm * scaleFactor();

        SFctConvert fctConvertPxFromMeter(
            /* fctConvertType */ EFctConvert_mMULxADDt,
            /* pPhysUnitSrc   */ &m_treeEntryMeter,
            /* pPhysUnitDst   */ &m_treeEntryPixel,
            /* physValM       */ fFac );

        m_treeEntryPixel.setFctConvertFromSIUnit(fctConvertPxFromMeter);

        emit scaleFactorChanged();
    }
}

//------------------------------------------------------------------------------
/*! @brief Returns the scale factors dividend.

    @return
        Scale factor dividend.
*/
double CUnitsLength::scaleFactor() const
//------------------------------------------------------------------------------
{
    return getReferenceValue("ScaleFactorDividend").getVal() / getReferenceValue("ScaleFactorDivisor").getVal();
}

//------------------------------------------------------------------------------
/*! @brief Returns the scale factors dividend.

    @return
        Scale factor dividend.
*/
int CUnitsLength::scaleFactorDividend() const
//------------------------------------------------------------------------------
{
    return static_cast<int>(getReferenceValue("ScaleFactorDividend").getVal());
}

//------------------------------------------------------------------------------
/*! @brief Returns the scale factors divisor.

    @return
        Scale factor divisor.
*/
int CUnitsLength::scaleFactorDivisor() const
//------------------------------------------------------------------------------
{
    return static_cast<int>(getReferenceValue("ScaleFactorDivisor").getVal());
}

/*==============================================================================
public: // instance methods (distinguish between metric system and pixels or dots)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Checks whether the given unit string is a metric unit.

    @return true, if the given unit string is a metric unit, false otherwise.
*/
bool CUnitsLength::isMetricUnit(const QString& i_strSymbolOrName) const
//------------------------------------------------------------------------------
{
    bool bIsMetricUnit = true;
    if (i_strSymbolOrName == px.symbol() || i_strSymbolOrName == px.unitName()) {
        bIsMetricUnit = false;
    }
    else if (i_strSymbolOrName == dots.symbol() || i_strSymbolOrName == dots.unitName()) {
        bIsMetricUnit = false;
    }
    return bIsMetricUnit;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the given unit is a metric unit.

    @return true, if the given unit string is a metric unit, false otherwise.
*/
bool CUnitsLength::isMetricUnit(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    bool bIsMetricUnit = true;
    if (i_unit == px || i_unit == dots) {
        bIsMetricUnit = false;
    }
    return bIsMetricUnit;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the given unit strings are all either metric or pixel units.

    @return true, if all given unit strings are metric units or all given unit strings
            are pixel units, false otherwise.
*/
bool CUnitsLength::unitsAreEitherMetricOrNot(
    const QString& i_strSymbolOrName1, const QString& i_strSymbolOrName2) const
//------------------------------------------------------------------------------
{
    bool bAllTheSame = false;
    if ((Units.Length.isMetricUnit(i_strSymbolOrName1) && Units.Length.isMetricUnit(i_strSymbolOrName2))
     || (!Units.Length.isMetricUnit(i_strSymbolOrName1) && !Units.Length.isMetricUnit(i_strSymbolOrName2)))
    {
        bAllTheSame = true;
    }
    return bAllTheSame;
}

//------------------------------------------------------------------------------
/*! @brief Checks whether the given units are all either metric or pixel units.

    @return true, if all given units are metric units or all given units are
            pixel units, false otherwise.
*/
bool CUnitsLength::unitsAreEitherMetricOrNot(
    const ZS::PhysVal::CUnit& i_unit1, const ZS::PhysVal::CUnit& i_unit2) const
//------------------------------------------------------------------------------
{
    bool bAllTheSame = false;
    if ((Units.Length.isMetricUnit(i_unit1) && Units.Length.isMetricUnit(i_unit2))
     || (!Units.Length.isMetricUnit(i_unit1) && !Units.Length.isMetricUnit(i_unit2)))
    {
        bAllTheSame = true;
    }
    return bAllTheSame;
}

/*******************************************************************************
class CUnitsAngle : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CUnitsAngle::CUnitsAngle( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Angle",
        /* strSIUnitName    */ "Radiant",
        /* strSIUnitSymbol  */ "rad",
        /* strFormulaSymbol */ Math::c_strSymbolPhi,
        /* bIsPowerRelated  */ false ),
    m_treeEntryRad(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Radiant",
        /* strSymbol      */ "rad",
        /* fMFromSI       */ 1.0 ),
    m_treeEntryDegree(
      /* pPhysSize      */ this,
      /* bIsLogarithmic */ false,
      /* strName        */ "Degree",
      /* strSymbol      */ Math::c_strSymbolDegree,
      /* fMFromSI       */ 180.0/Math::c_fPI ),
    Rad(m_treeEntryRad),
    Degree(m_treeEntryDegree)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(false);

    // To allow "short" unit strings like "°" we add shortcuts to each unit.
    //m_pTree->addShortcut(&m_treeEntryDegree, QString::fromLatin1("°"));
    m_pTree->addShortcut(&m_treeEntryDegree, ZS::System::Math::c_strSymbolDegree);

} // ctor

//------------------------------------------------------------------------------
CUnitsAngle::~CUnitsAngle()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CUnits : public CIdxTreeUnits
*******************************************************************************/

#ifdef ZSSys_MemLeakDump_h
#pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
#pragma warning( disable : 4005 )
#define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0
#endif

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates the physical units index tree with units used by the drawing library.
*/
CUnits::CUnits() :
//------------------------------------------------------------------------------
    CIdxTreeUnits(new CIdxTreeEntry(CIdxTreeEntry::EEntryType::Root, "Units")),
    Length(root()),
    Angle(root())
{
} // ctor

//------------------------------------------------------------------------------
/*! Destroys the physical size.
*/
CUnits::~CUnits()
//------------------------------------------------------------------------------
{
} // dtor

#ifdef ZSSys_MemLeakDump_h
#pragma warning( default : 4005 )
#pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
#endif
