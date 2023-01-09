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

#include "PhysSizes/Electricity/PhysSizeVoltage.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeVoltage : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeVoltage::CPhysSizeVoltage( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Voltage",
        /* strSIUnitName    */ "Volt",
        /* strSIUnitSymbol  */ "V",
        /* strFormulaSymbol */ "U",
        /* bIsPowerRelated  */ false ),
    PicoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    NanoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    Volt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    MegaVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    dBVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBVolt",
        /* strSymbol      */ "dBV",
        /* fRefVal        */ 1.0 ),
    dBu(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB(0,775V)",
        /* strSymbol      */ "dBu",
        /* fRefVal        */ c_fRefValdBu_0775V ),
    dBMilliVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMilli) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMilli) + "V",
        /* fRefVal        */ c_fFactorMilli ),
    dBMicroVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMicro) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMicro) + "V",
        /* fRefVal        */ c_fFactorMicro ),
    dBNanoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strNano) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixNano) + "V",
        /* fRefVal        */ c_fFactorNano ),
    dBPicoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strPico) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixPico) + "V",
        /* fRefVal        */ c_fFactorPico ),
    pV(PicoVolt),
    nV(NanoVolt),
    uV(MicroVolt),
    mV(MilliVolt),
    V(Volt),
    kV(KiloVolt),
    MV(MegaVolt),
    dBV(dBVolt),
    dBmV(dBMilliVolt),
    dBuV(dBMicroVolt),
    dBnV(dBNanoVolt),
    dBpV(dBPicoVolt)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeVoltage::CPhysSizeVoltage( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Voltage",
        /* strSIUnitName    */ "Volt",
        /* strSIUnitSymbol  */ "V",
        /* strFormulaSymbol */ "U",
        /* bIsPowerRelated  */ false ),
    PicoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    NanoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    Volt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    MegaVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    dBVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBVolt",
        /* strSymbol      */ "dBV",
        /* fRefVal        */ 1.0 ),
    dBu(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB(0,775V)",
        /* strSymbol      */ "dBu",
        /* fRefVal        */ c_fRefValdBu_0775V ),
    dBMilliVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMilli) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMilli) + "V",
        /* fRefVal        */ c_fFactorMilli ),
    dBMicroVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strMicro) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMicro) + "V",
        /* fRefVal        */ c_fFactorMicro ),
    dBNanoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strNano) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixNano) + "V",
        /* fRefVal        */ c_fFactorNano ),
    dBPicoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + QString(c_strPico) + "Volt",
        /* strSymbol      */ "dB" + QString(c_strPrefixPico) + "V",
        /* fRefVal        */ c_fFactorPico ),
    pV(PicoVolt),
    nV(NanoVolt),
    uV(MicroVolt),
    mV(MilliVolt),
    V(Volt),
    kV(KiloVolt),
    MV(MegaVolt),
    dBV(dBVolt),
    dBmV(dBMilliVolt),
    dBuV(dBMicroVolt),
    dBnV(dBNanoVolt),
    dBpV(dBPicoVolt)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeVoltage::~CPhysSizeVoltage()
//------------------------------------------------------------------------------
{
} // dtor
