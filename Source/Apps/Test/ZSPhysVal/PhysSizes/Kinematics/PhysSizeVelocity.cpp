/*******************************************************************************

Copyright 2004 - 2022 by ZeusSoft, Ing. Buero Bauer
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

#include "PhysSizes/Kinematics/PhysSizeVelocity.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeVelocity : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeVelocity::CPhysSizeVelocity( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Velocity",
        /* strSIUnitName    */ "Meter Per Second",
        /* strSIUnitSymbol  */ "m/s",
        /* strFormulaSymbol */ "v",
        /* bIsPowerRelated  */ false ),
    m_treeEntryMetersPerSecond(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKilometersPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Kilometer Per Hour",
        /* strSymbol      */ "km/h",
        /* fMFromSI       */ 3.6 ),
    m_treeEntryMilesPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Miles Per Hour",
        /* strSymbol      */ "mi/h",
        /* fMFromSI       */ 3600.0/1.609344e3 ),
    MetersPerSecond(m_treeEntryMetersPerSecond),
    KilometersPerHour(m_treeEntryKilometersPerHour),
    MilesPerHour(m_treeEntryMilesPerHour),
    mps(m_treeEntryMetersPerSecond),
    kmph(m_treeEntryKilometersPerHour),
    milesph(m_treeEntryMilesPerHour)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeVelocity::CPhysSizeVelocity( CPhysScienceFieldTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Velocity",
        /* strSIUnitName    */ "Meter Per Second",
        /* strSIUnitSymbol  */ "m/s",
        /* strFormulaSymbol */ "v",
        /* bIsPowerRelated  */ false ),
    m_treeEntryMetersPerSecond(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKilometersPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Kilometer Per Hour",
        /* strSymbol      */ "km/h",
        /* fMFromSI       */ 3.6 ),
    m_treeEntryMilesPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Miles Per Hour",
        /* strSymbol      */ "mi/h",
        /* fMFromSI       */ 3600.0/1.609344e3 ),
    MetersPerSecond(m_treeEntryMetersPerSecond),
    KilometersPerHour(m_treeEntryKilometersPerHour),
    MilesPerHour(m_treeEntryMilesPerHour),
    mps(m_treeEntryMetersPerSecond),
    kmph(m_treeEntryKilometersPerHour),
    milesph(m_treeEntryMilesPerHour)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeVelocity::~CPhysSizeVelocity()
//------------------------------------------------------------------------------
{
} // dtor
