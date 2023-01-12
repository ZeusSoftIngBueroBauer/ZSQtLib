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

#include "Units/Geometry/PhysSizeLength.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeLength : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeLength::CPhysSizeLength( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "l",
        /* bIsPowerRelated  */ false ),
    m_treeEntryPicoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_treeEntryNanoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
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
    m_treeEntryFoot(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Foot",
        /* strSymbol      */ "ft",
        /* fMFromSI       */ 1.0/(12.0*0.0254) ),
    m_treeEntryYard(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Yard",
        /* strSymbol      */ "yd",
        /* fMFromSI       */ 1.0/(36.0*0.0254) ),
    m_treeEntryMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Mile",
        /* strSymbol      */ "mi",
        /* fMFromSI       */ 1.0/1609.344 ),
    m_treeEntryNauticalMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Nautical Mile",
        /* strSymbol      */ "sm",
        /* fMFromSI       */ 1.0/1852.0 ),
    PicoMeter(m_treeEntryPicoMeter),
    NanoMeter(m_treeEntryNanoMeter),
    MicroMeter(m_treeEntryMicroMeter),
    MilliMeter(m_treeEntryMilliMeter),
    CentiMeter(m_treeEntryCentiMeter),
    DeziMeter(m_treeEntryDeziMeter),
    Meter(m_treeEntryMeter),
    KiloMeter(m_treeEntryKiloMeter),
    Inch(m_treeEntryInch),
    Foot(m_treeEntryFoot),
    Yard(m_treeEntryYard),
    Mile(m_treeEntryMile),
    NauticalMile(m_treeEntryNauticalMile),
    pm(m_treeEntryPicoMeter),
    nm(m_treeEntryNanoMeter),
    um(m_treeEntryMicroMeter),
    mm(m_treeEntryMilliMeter),
    cm(m_treeEntryCentiMeter),
    dm(m_treeEntryDeziMeter),
    m(m_treeEntryMeter),
    km(m_treeEntryKiloMeter),
    ft(m_treeEntryFoot),
    yd(m_treeEntryYard)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    m_treeEntryPicoMeter. setNextLowerHigherUnits( nullptr,                &m_treeEntryNanoMeter  );
    m_treeEntryNanoMeter. setNextLowerHigherUnits( &m_treeEntryPicoMeter,  &m_treeEntryMicroMeter );
    m_treeEntryMicroMeter.setNextLowerHigherUnits( &m_treeEntryNanoMeter,  &m_treeEntryMilliMeter );
    m_treeEntryMilliMeter.setNextLowerHigherUnits( &m_treeEntryMicroMeter, &m_treeEntryMeter      );
    m_treeEntryMeter.     setNextLowerHigherUnits( &m_treeEntryMilliMeter, &m_treeEntryKiloMeter  );
    m_treeEntryKiloMeter. setNextLowerHigherUnits( &m_treeEntryMeter,      nullptr                );

} // ctor

//------------------------------------------------------------------------------
CPhysSizeLength::~CPhysSizeLength()
//------------------------------------------------------------------------------
{
} // dtor
