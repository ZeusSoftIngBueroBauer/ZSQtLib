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

#include "ZSDraw/Drawing/ZSDrawUnits.h"
#include "ZSSys/ZSSysMemLeakDump.h"

#include <QtGui/qpaintdevice.h>

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
    m_treeEntryPixelX(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "PixelX",
        /* strSymbol      */ "pxX" ),
    m_treeEntryPixelY(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "PixelY",
        /* strSymbol      */ "pxY" ),
    m_treeEntryDotsX(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "DotsX",
        /* strSymbol      */ "dotsX" ),
    m_treeEntryDotsY(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "DotsY",
        /* strSymbol      */ "dotsY" ),
    um(m_treeEntryMicroMeter),
    mm(m_treeEntryMilliMeter),
    cm(m_treeEntryCentiMeter),
    dm(m_treeEntryDeziMeter),
    m(m_treeEntryMeter),
    km(m_treeEntryKiloMeter),
    in(m_treeEntryInch),
    pxX(m_treeEntryPixelX),
    pxY(m_treeEntryPixelY),
    dotsX(m_treeEntryDotsX),
    dotsY(m_treeEntryDotsY)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
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
    m_pTree->addShortcut(&m_treeEntryPixelX, "pxX");
    m_pTree->addShortcut(&m_treeEntryPixelY, "pxY");
    m_pTree->addShortcut(&m_treeEntryDotsX, "dotsX");
    m_pTree->addShortcut(&m_treeEntryDotsY, "dotsY");

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
           for both directions (horizontal X, vertical Y) as reference values with
           the names "PxpiX" and "PxpiY" to the unit group.

    @param [in] i_fPxpiX
        Resolution of a screen pixel in pixels/inches for horizonal direction.
    @param [in] i_fPxpiY
        Resolution of a screen pixel in pixels/inches for vertical direction.
*/
void CUnitsLength::setPxpis( double i_fPxpiX, double i_fPxpiY )
//------------------------------------------------------------------------------
{
    setReferenceValue("PxpiX", i_fPxpiX);
    setReferenceValue("PxpiY", i_fPxpiY);

    // 1 Inch = 2.54 cm
    // Factor to convert inches into mm: 25.4
    // Factor to convert inches into cm: 2.54
    // Factor to convert inches to m: 0.0254
    // Factor to convert m into inches: 1/0.0254
    double fipm = m_treeEntryInch.getFactorConvertFromSIUnit();

    // Factor to convert Px/inch to Px/m: 1/0.0254
    double fpxmX = fipm * i_fPxpiX;
    double fpxmY = fipm * i_fPxpiY;

    SFctConvert fctConvertPxXFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryPixelX,
        /* physValM       */ fpxmX );
    SFctConvert fctConvertPxYFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryPixelY,
        /* physValM       */ fpxmY );

    m_treeEntryPixelX.setFctConvertFromSIUnit(fctConvertPxXFromMeter);
    m_treeEntryPixelY.setFctConvertFromSIUnit(fctConvertPxYFromMeter);
}

//------------------------------------------------------------------------------
/*! Returns the logical resolution of a pixel on the screen for the specified
    direction in pixel/inch.

    @param [in] i_orientation
        Direction (Vertical or Horizontal) for which the resolution is returned.

    @return
        Resolution in pixels per inch.
*/
double CUnitsLength::pxpi( EOrientation i_orientation ) const
//------------------------------------------------------------------------------
{
    QString strRefValName;
    if( i_orientation == EOrientation::Horizontal ) {
        strRefValName = "PxpiX";
    } else if( i_orientation == EOrientation::Vertical ) {
        strRefValName = "PxpiY";
    }
    return getReferenceValue(strRefValName).getVal();
}

/*==============================================================================
public: // instance methods (resolution of printer, dots per inches)
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief Sets the resolution of a dot on the print device in Dots/inch for both
           directions (horizontal X, vertical Y) as reference values with the
           names "DpiX" and "DpiY" to the unit group.

    @param [in] i_fDpiX
        Resolution of a dot in Dots/inch for horizonal direction.
    @param [in] i_fDpiY
        Resolution of a dot in Dots/inch for vertical direction.
*/
void CUnitsLength::setDpis( double i_fDpiX, double i_fDpiY )
//------------------------------------------------------------------------------
{
    setReferenceValue("DpiX", i_fDpiX);
    setReferenceValue("DpiY", i_fDpiY);

    // 1 Inch = 2.54 cm
    // Factor to convert inches into mm: 25.4
    // Factor to convert inches into cm: 2.54
    // Factor to convert inches to m: 0.0254
    // Factor to convert m into inches: 1/0.0254
    double fipm = m_treeEntryInch.getFactorConvertFromSIUnit();

    // Factor to convert Dots/inch to Dots/m: 1/0.0254
    double fdmX = fipm * i_fDpiX;
    double fdmY = fipm * i_fDpiY;

    SFctConvert fctConvertDotsXFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryDotsX,
        /* physValM       */ fdmX );
    SFctConvert fctConvertDotsYFromMeter(
        /* fctConvertType */ EFctConvert_mMULxADDt,
        /* pPhysUnitSrc   */ &m_treeEntryMeter,
        /* pPhysUnitDst   */ &m_treeEntryDotsY,
        /* physValM       */ fdmY );

    m_treeEntryDotsX.setFctConvertFromSIUnit(fctConvertDotsXFromMeter);
    m_treeEntryDotsY.setFctConvertFromSIUnit(fctConvertDotsYFromMeter);
}

//------------------------------------------------------------------------------
/*! Returns the resolution of a dot on the print device for the specified
    direction in dots/inch.

    @param [in] i_orientation
        Direction (Vertical or Horizontal) for which the resolution is returned.

    @return
        Resolution in dots per inch.
*/
double CUnitsLength::dpi( EOrientation i_orientation ) const
//------------------------------------------------------------------------------
{
    QString strRefValName;
    if( i_orientation == EOrientation::Horizontal ) {
        strRefValName = "DpiX";
    } else if( i_orientation == EOrientation::Vertical ) {
        strRefValName = "DpiY";
    }
    return getReferenceValue(strRefValName).getVal();
}

/*==============================================================================
public: // instance methods (distinguish between metric system and pixels or dots)
==============================================================================*/

//------------------------------------------------------------------------------
bool CUnitsLength::isMetricUnit(const QString& i_strSymbolOrName) const
//------------------------------------------------------------------------------
{
    bool bIsMetricUnit = true;
    if( i_strSymbolOrName == pxX.symbol() || i_strSymbolOrName == pxX.unitName() ) {
        bIsMetricUnit = false;
    }
    else if( i_strSymbolOrName == pxY.symbol() || i_strSymbolOrName == pxY.unitName() ) {
        bIsMetricUnit = false;
    }
    if( i_strSymbolOrName == dotsX.symbol() || i_strSymbolOrName == dotsX.unitName() ) {
        bIsMetricUnit = false;
    }
    else if( i_strSymbolOrName == dotsY.symbol() || i_strSymbolOrName == dotsY.unitName() ) {
        bIsMetricUnit = false;
    }
    return bIsMetricUnit;
}


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
    Length(root())
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
