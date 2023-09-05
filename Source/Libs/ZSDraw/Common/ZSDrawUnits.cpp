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

#include <QtGui/qpaintdevice.h>

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
/*! @brief Sets the logical resolution of a pixel on the screen in pixel/inch
           as reference value with the name "Pxpi" to the unit group.

    Different resolutions for horizontal and vertical directions are not supported.
    Anyway very often several monitors are used whose resolution may be different.

    @param [in] i_fPxpi
        Resolution of a screen pixel in pixels/inches.
*/
void CUnitsLength::setPxpi( double i_fPxpi )
//------------------------------------------------------------------------------
{
    setReferenceValue("Pxpi", i_fPxpi);

    // 1 Inch = 2.54 cm
    // Factor to convert inches into mm: 25.4
    // Factor to convert inches into cm: 2.54
    // Factor to convert inches to m: 0.0254
    // Factor to convert m into inches: 1/0.0254
    double fipm = m_treeEntryInch.getFactorConvertFromSIUnit();

    // Factor to convert Px/inch to Px/m: 1/0.0254
    double fpxm = fipm * i_fPxpi;

    SFctConvert fctConvertPxFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryPixel,
        /* physValM       */ fpxm );

    m_treeEntryPixel.setFctConvertFromSIUnit(fctConvertPxFromMeter);
}

//------------------------------------------------------------------------------
/*! @brief Returns the logical resolution of a pixel on the screen.

    @return
        Resolution in pixels per inch.
*/
double CUnitsLength::pxpi() const
//------------------------------------------------------------------------------
{
    return getReferenceValue("Pxpi").getVal();
}

/*==============================================================================
public: // instance methods (resolution of printer, dots per inches)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of a dot on the print device in Dots/inch as
           reference value with the names "Dpi" to the unit group.

    Different resolutions for horizontal and vertical directions are not supported.

    @param [in] i_fDpi
        Resolution of a dot in Dots/inch.
*/
void CUnitsLength::setDpi( double i_fDpi )
//------------------------------------------------------------------------------
{
    setReferenceValue("Dpi", i_fDpi);

    // 1 Inch = 2.54 cm
    // Factor to convert inches into mm: 25.4
    // Factor to convert inches into cm: 2.54
    // Factor to convert inches to m: 0.0254
    // Factor to convert m into inches: 1/0.0254
    double fipm = m_treeEntryInch.getFactorConvertFromSIUnit();

    // Factor to convert Dots/inch to Dots/m: 1/0.0254
    double fdm = fipm * i_fDpi;

    SFctConvert fctConvertDotsFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryDots,
        /* physValM       */ fdm );

    m_treeEntryDots.setFctConvertFromSIUnit(fctConvertDotsFromMeter);
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of a dot on the print device in dots/inch.

    @return
        Resolution in dots per inch.
*/
double CUnitsLength::dpi() const
//------------------------------------------------------------------------------
{
    return getReferenceValue("Dpi").getVal();
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Returns the screen resolution of a pixel in the given unit.

    The screen resolution is usually defined in pixels per inch or pixels per mm.
    To format values correctly with the number of significant digits the resolution
    normed to a metric unit is needed. E.g. if the screen resolution would be 10 pixels
    per mm, the number of trailing digits to indicate a value in mm would be 1 and
    e.g. 7 pixels would have to be rounded to 0.7 mm.

    @param [in] i_unit (default mm)
        Unit in which the resolution has to be returned.
        Please note that it does not make sense to pass pixels or dots as the
        desired unit as the screen resolution or the device resolution in pixels
        or dots is always 1.0.

    @return Screen resolution normed to the given metric unit.
*/
CPhysValRes CUnitsLength::physValResPerPx(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    double fResVal = 0.1;
    if (i_unit == px || i_unit == dots) {
        fResVal = 1.0;
    }
    else {
        // Resolution in inches:
        fResVal = 1.0 / pxpi();
        if (i_unit != in) {
            fResVal = in.convertValue(fResVal, i_unit);
        }
    }
    return CPhysValRes(fResVal, i_unit);
}

//------------------------------------------------------------------------------
/*! @brief Returns the resolution of a dot on the print device in the given unit.

    The print device resolution is usually defined in dots per inch or dots per mm.
    To format values correctly with the number of significant digits the resolution
    normed to a metric unit is needed. E.g. if the print device resolution would be 10 dots
    per mm, the number of trailing digits to indicate a value in mm would be 1 and
    e.g. 7 dots would have to be rounded to 0.7 mm.

    @param [in] i_unit (default mm)
        Unit in which the resolution has to be returned.
        Please note that it does not make sense to pass pixels or dots as the
        desired unit as the screen resolution or the device resolution in pixels
        or dots is always 1.0.

    @return Screen resolution normed to the given metric unit.
*/
CPhysValRes CUnitsLength::physValResPerDot(const CUnit& i_unit) const
//------------------------------------------------------------------------------
{
    double fResVal = 0.1;
    if (i_unit == px || i_unit == dots) {
        fResVal = 1.0;
    }
    else {
        // Resolution in inches:
        fResVal = 1.0 / dpi();
        if (i_unit != in) {
            fResVal = in.convertValue(fResVal, i_unit);
        }
    }
    return CPhysValRes(fResVal, i_unit);
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


/*******************************************************************************
class CUnitsAngle : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // class members

==============================================================================*/

const QString CUnitsAngle::c_strSymbolPhi(QChar(0x03C6));

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
        /* strFormulaSymbol */ QString(QChar(0x03C6)),
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
      /* strSymbol      */ QString::fromLatin1("�"),
      /* fMFromSI       */ 180.0/Math::c_fPI ),
    Rad(m_treeEntryRad),
    Degree(m_treeEntryDegree)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(false);

    // To allow "short" unit strings like "�" we add shortcuts to each unit.
    m_pTree->addShortcut(&m_treeEntryDegree, QString::fromLatin1("�"));

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