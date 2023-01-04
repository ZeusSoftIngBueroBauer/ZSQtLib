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
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

#if 0

using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Electricity;


/*******************************************************************************
class CPhysSizeVoltage : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeVoltage::CPhysSizeVoltage() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldElectricity,
        /* strName          */ "Voltage",
        /* strSIUnitName    */ "Volt",
        /* strSIUnitSymbol  */ "V",
        /* strFormulaSymbol */ "U",
        /* bIsPowerRelated  */ false ),
    m_physUnitPicoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_physUnitNanoVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_physUnitMicroVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_physUnitMilliVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_physUnitVolt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKiloVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_physUnitMegaVolt(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    m_physUnitdBVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBVolt",
        /* strSymbol      */ "dBV",
        /* fRefVal        */ 1.0 ),
    m_physUnitdBu(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB(0.775V)",
        /* strSymbol      */ "dBu",
        /* fRefVal        */ c_fRefValdBu_0775V ),
    m_physUnitdBMilliVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + c_strMilli + "Volt",
        /* strSymbol      */ "dB" + c_strPrefixMilli + "V",
        /* fRefVal        */ c_fFactorMilli ),
    m_physUnitdBMicroVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + c_strMicro + "Volt",
        /* strSymbol      */ "dB" + c_strPrefixMicro + "V",
        /* fRefVal        */ c_fFactorMicro ),
    m_physUnitdBNanoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + c_strNano + "Volt",
        /* strSymbol      */ "dB" + c_strPrefixNano + "V",
        /* fRefVal        */ c_fFactorNano ),
    m_physUnitdBPicoVolt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dB" + c_strPico + "Volt",
        /* strSymbol      */ "dB" + c_strPrefixPico + "V",
        /* fRefVal        */ c_fFactorPico )
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

#endif
