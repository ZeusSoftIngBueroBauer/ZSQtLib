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
        /* strFormulaSymbol */ "l",
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
    um(m_treeEntryMicroMeter),
    mm(m_treeEntryMilliMeter),
    cm(m_treeEntryCentiMeter),
    dm(m_treeEntryDeziMeter),
    m(m_treeEntryMeter),
    km(m_treeEntryKiloMeter)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // To allow "short" unit strings like "mm" we add shortcuts to each unit.
    m_pTree->addShortcut(&m_treeEntryMicroMeter, "um");
    m_pTree->addShortcut(&m_treeEntryMilliMeter, "mm");
    m_pTree->addShortcut(&m_treeEntryCentiMeter, "cm");
    m_pTree->addShortcut(&m_treeEntryDeziMeter, "dm");
    m_pTree->addShortcut(&m_treeEntryMeter, "m");
    m_pTree->addShortcut(&m_treeEntryKiloMeter, "km");

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the physical size.
*/
CUnitsLength::~CUnitsLength()
//------------------------------------------------------------------------------
{
} // dtor


/*******************************************************************************
class CUnitsGraphDevice : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! Creates the physical size for the graphical device.

    @param [in] i_pParentBranch
        Parent branch in the index tree for this physical size.
*/
CUnitsGraphDevice::CUnitsGraphDevice(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* scienceField     */ i_pParentBranch,
        /* strName          */ "GraphDevice",
        /* strSIUnitName    */ "Pixel",
        /* strSIUnitSymbol  */ "px",
        /* strFormulaSymbol */ "px",
        /* bIsPowerRelated  */ false ),
    m_treeEntryPixel(
        /* pPhysSize      */ this,
        /* strPrefix */ "" ),
    m_arfDpmms(CEnumDirection::count()),
    px(m_treeEntryPixel)
{
    m_arfDpmms[static_cast<int>(EDirection::Horizontal)] = 3.78;
    m_arfDpmms[static_cast<int>(EDirection::Vertical)] = 3.78;

    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // To allow "short" unit strings like "Hz" we add shortcuts to each unit.
    m_pTree->addShortcut(&m_treeEntryPixel, "px");

} // ctor

//------------------------------------------------------------------------------
/*! Destroys the physical size.
*/
CUnitsGraphDevice::~CUnitsGraphDevice()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
/*! Sets the resolution of a pixel on the screen for the both directions
    in dots per milli meter.

    @param [in] i_fDpmmX
        Pixel resolution in dots per milli meter (= px/mm) for the horizontal direction.
    @param [in] i_fDpmmY
        Pixel resolution in dots per milli meter (= px/mm) for the vertical direction.
*/
void CUnitsGraphDevice::setDpmms( double i_fDpmmX, double i_fDpmmY )
//------------------------------------------------------------------------------
{
    m_arfDpmms[static_cast<int>(EDirection::Horizontal)] = i_fDpmmX;
    m_arfDpmms[static_cast<int>(EDirection::Vertical)] = i_fDpmmY;
}

//------------------------------------------------------------------------------
/*! Sets the resolution of a pixel on the screen for the specified direction
    in dots per milli meter.

    @param [in] i_direction
        Direction (Vertical or Horizontal) for which the resolution is set.
    @param [in] i_fDpmm
        Pixel resolution in dots per milli meter (= px/mm).
*/
void CUnitsGraphDevice::setDpmm( EDirection i_direction, double i_fDpmm )
//------------------------------------------------------------------------------
{
    m_arfDpmms[static_cast<int>(i_direction)] = i_fDpmm;
}

//------------------------------------------------------------------------------
/*! Returns the resolution of a pixel on the screen for the specified direction
    in dots per milli meter.

    @param [in] i_direction
        Direction (Vertical or Horizontal) for which the resolution is returned.

    @return
        Pixel resolution in dots per milli meter (= px/mm).
*/
double CUnitsGraphDevice::dpmm( EDirection i_direction ) const
//------------------------------------------------------------------------------
{
    return m_arfDpmms[static_cast<int>(i_direction)];
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
    CIdxTreeUnits(new CIdxTreeEntry(EIdxTreeEntryType::Root, "Units")),
    Length(root()),
    GraphDevice(root())
{
    Length.addFctConvert(
        /* pPhysSizeDst   */ &GraphDevice,
        /* pPhysSizeRef   */ &GraphDevice,
        /* fctConvert     */ EFctConvert_xMULr );

    GraphDevice.addFctConvert(
        /* pPhysSizeDst   */ &Length,
        /* pPhysSizeRef   */ &GraphDevice,
        /* fctConvert     */ EFctConvert_xDIVr );

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
