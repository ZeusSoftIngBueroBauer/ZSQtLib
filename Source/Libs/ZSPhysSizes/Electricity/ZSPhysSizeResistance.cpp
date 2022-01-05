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

#include "ZSPhysSizes/Electricity/ZSPhysSizeResistance.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Electricity;


/*******************************************************************************
class CPhysSizeResistance : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeResistance::CPhysSizeResistance() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldElectricity,
        /* strName          */ "Resistance",
        /* strSIUnitName    */ "Ohm",
        /* strSIUnitSymbol  */ c_strSymbolOhm,
        /* strFormulaSymbol */ "R",
        /* bIsPowerRelated  */ false ),
    m_physUnitNanoOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_physUnitMicroOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_physUnitMilliOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_physUnitOhm(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKiloOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_physUnitMegaOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeResistance::~CPhysSizeResistance()
//------------------------------------------------------------------------------
{
} // dtor

/*==============================================================================
public: // overridables of base class CPhysSize
==============================================================================*/

//------------------------------------------------------------------------------
double CPhysSizeResistance::getRefVal( CPhysUnit* i_pPhysUnitRef ) const
//------------------------------------------------------------------------------
{
    double fValRef = 50.0; // Ohm

    if( i_pPhysUnitRef != nullptr && i_pPhysUnitRef != &m_physUnitOhm )
    {
        fValRef = m_physUnitOhm.convertValue(fValRef,i_pPhysUnitRef);
    }
    return fValRef;
}
