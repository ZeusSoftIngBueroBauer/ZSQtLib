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

#include "PhysSizes/Electricity/PhysSizePower.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

#if 0

using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Electricity;


/*******************************************************************************
class CPhysSizePower : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizePower::CPhysSizePower() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldElectricity,
        /* strName          */ "Power",
        /* strSIUnitName    */ "Watt",
        /* strSIUnitSymbol  */ "W",
        /* strFormulaSymbol */ "P",
        /* bIsPowerRelated  */ true ),
    m_physUnitPicoWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_physUnitNanoWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_physUnitMicroWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_physUnitMilliWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_physUnitWatt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKiloWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_physUnitMegaWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    m_physUnitGigaWatt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixGiga ),
    m_physUnitdBWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBWatt",
        /* strSymbol      */ "dBW",
        /* fRefVal        */ 1.0 ),
    m_physUnitdBMilliWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBMilliWatt",
        /* strSymbol      */ "dB" + c_strPrefixMilli,
        /* fRefVal        */ c_fFactorMilli ),
    m_physUnitdBMicroWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBMicroWatt",
        /* strSymbol      */ "dB" + c_strPrefixMicro + "W",
        /* fRefVal        */ c_fFactorMicro ),
    m_physUnitdBNanoWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBNanoWatt",
        /* strSymbol      */ "dB" + c_strPrefixNano + "W",
        /* fRefVal        */ c_fFactorNano ),
    m_physUnitdBPicoWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBPicoWatt",
        /* strSymbol      */ "dB" + c_strPrefixPico + "W",
        /* fRefVal        */ c_fFactorPico )
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

#endif
