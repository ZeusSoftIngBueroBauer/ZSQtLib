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

#include "Units/Electricity/PhysSizePower.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizePower : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizePower::CPhysSizePower( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Power",
        /* strSIUnitName    */ "Watt",
        /* strSIUnitSymbol  */ "W",
        /* strFormulaSymbol */ "P",
        /* bIsPowerRelated  */ true ),
    m_treeEntryPicoWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_treeEntryNanoWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_treeEntryMicroWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_treeEntryWatt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKiloWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_treeEntryMegaWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    m_treeEntryGigaWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixGiga ),
    m_treeEntrydBWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBWatt",
        /* strSymbol      */ "dBW",
        /* fRefVal        */ 1.0 ),
    m_treeEntrydBMilliWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBMilliWatt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMilli),
        /* fRefVal        */ c_fFactorMilli ),
    m_treeEntrydBMicroWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBMicroWatt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMicro) + "W",
        /* fRefVal        */ c_fFactorMicro ),
    m_treeEntrydBNanoWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBNanoWatt",
        /* strSymbol      */ "dB" + QString(c_strPrefixNano) + "W",
        /* fRefVal        */ c_fFactorNano ),
    m_treeEntrydBPicoWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBPicoWatt",
        /* strSymbol      */ "dB" + QString(c_strPrefixPico) + "W",
        /* fRefVal        */ c_fFactorPico ),
    PicoWatt(m_treeEntryPicoWatt),
    NanoWatt(m_treeEntryNanoWatt),
    MicroWatt(m_treeEntryMicroWatt),
    MilliWatt(m_treeEntryMilliWatt),
    Watt(m_treeEntryWatt),
    KiloWatt(m_treeEntryKiloWatt),
    MegaWatt(m_treeEntryMegaWatt),
    GigaWatt(m_treeEntryGigaWatt),
    dBWatt(m_treeEntrydBWatt),
    dBMilliWatt(m_treeEntrydBMilliWatt),
    dBMicroWatt(m_treeEntrydBMicroWatt),
    dBNanoWatt(m_treeEntrydBNanoWatt),
    dBPicoWatt(m_treeEntrydBPicoWatt),
    pW(m_treeEntryPicoWatt),
    nW(m_treeEntryNanoWatt),
    uW(m_treeEntryMicroWatt),
    mW(m_treeEntryMilliWatt),
    W(m_treeEntryWatt),
    kW(m_treeEntryKiloWatt),
    MW(m_treeEntryMegaWatt),
    GW(m_treeEntryGigaWatt),
    dBW(m_treeEntrydBWatt),
    dBm(m_treeEntrydBMilliWatt),
    dBmW(m_treeEntrydBMilliWatt),
    dBuW(m_treeEntrydBMicroWatt),
    dBnW(m_treeEntrydBNanoWatt),
    dBpW(m_treeEntrydBPicoWatt)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizePower::~CPhysSizePower()
//------------------------------------------------------------------------------
{
} // dtor
