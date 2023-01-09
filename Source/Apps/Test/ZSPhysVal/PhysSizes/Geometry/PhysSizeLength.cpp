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

#include "PhysSizes/Geometry/PhysSizeLength.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeLength : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeLength::CPhysSizeLength( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "l",
        /* bIsPowerRelated  */ false ),
    PicoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    NanoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    CentiMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixCenti ),
    DeziMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixDezi ),
    Meter(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    Inch(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Inch",
        /* strSymbol      */ "in",
        /* fMFromSI       */ 1.0/0.0254 ),
    Foot(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Foot",
        /* strSymbol      */ "ft",
        /* fMFromSI       */ 1.0/(12.0*0.0254) ),
    Yard(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Yard",
        /* strSymbol      */ "yd",
        /* fMFromSI       */ 1.0/(36.0*0.0254) ),
    Mile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Mile",
        /* strSymbol      */ "mi",
        /* fMFromSI       */ 1.0/1609.344 ),
    NauticalMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Nautical Mile",
        /* strSymbol      */ "sm",
        /* fMFromSI       */ 1.0/1852.0 ),
    pm(PicoMeter),
    nm(NanoMeter),
    um(MicroMeter),
    mm(MilliMeter),
    cm(CentiMeter),
    dm(DeziMeter),
    m(Meter),
    km(KiloMeter),
    ft(Foot),
    yd(Yard)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    PicoMeter. setNextLowerHigherUnits( nullptr,     &NanoMeter  );
    NanoMeter. setNextLowerHigherUnits( &PicoMeter,  &MicroMeter );
    MicroMeter.setNextLowerHigherUnits( &NanoMeter,  &MilliMeter );
    MilliMeter.setNextLowerHigherUnits( &MicroMeter, &Meter      );
    Meter.     setNextLowerHigherUnits( &MilliMeter, &KiloMeter  );
    KiloMeter. setNextLowerHigherUnits( &Meter,      nullptr     );

} // ctor

//------------------------------------------------------------------------------
CPhysSizeLength::CPhysSizeLength( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "l",
        /* bIsPowerRelated  */ false ),
    PicoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    NanoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    CentiMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixCenti ),
    DeziMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixDezi ),
    Meter(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    Inch(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Inch",
        /* strSymbol      */ "in",
        /* fMFromSI       */ 1.0/0.0254 ),
    Foot(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Foot",
        /* strSymbol      */ "ft",
        /* fMFromSI       */ 1.0/(12.0*0.0254) ),
    Yard(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Yard",
        /* strSymbol      */ "yd",
        /* fMFromSI       */ 1.0/(36.0*0.0254) ),
    Mile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Mile",
        /* strSymbol      */ "mi",
        /* fMFromSI       */ 1.0/1609.344 ),
    NauticalMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Nautical Mile",
        /* strSymbol      */ "sm",
        /* fMFromSI       */ 1.0/1852.0 ),
    pm(PicoMeter),
    nm(NanoMeter),
    um(MicroMeter),
    mm(MilliMeter),
    cm(CentiMeter),
    dm(DeziMeter),
    m(Meter),
    km(KiloMeter),
    ft(Foot),
    yd(Yard)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    PicoMeter. setNextLowerHigherUnits( nullptr,     &NanoMeter  );
    NanoMeter. setNextLowerHigherUnits( &PicoMeter,  &MicroMeter );
    MicroMeter.setNextLowerHigherUnits( &NanoMeter,  &MilliMeter );
    MilliMeter.setNextLowerHigherUnits( &MicroMeter, &Meter      );
    Meter.     setNextLowerHigherUnits( &MilliMeter, &KiloMeter  );
    KiloMeter. setNextLowerHigherUnits( &Meter,      nullptr     );

} // ctor

//------------------------------------------------------------------------------
CPhysSizeLength::~CPhysSizeLength()
//------------------------------------------------------------------------------
{
} // dtor
