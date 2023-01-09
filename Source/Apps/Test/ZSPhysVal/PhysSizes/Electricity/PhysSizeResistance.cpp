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

#include "PhysSizes/Electricity/PhysSizeResistance.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeResistance : public CPhysSize
*******************************************************************************/

const QString CPhysSizeResistance::c_strSymbolOhm(QChar(0x03A9));

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeResistance::CPhysSizeResistance( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pIdxTree         */ i_pIdxTree,
        /* strName          */ "Resistance",
        /* strSIUnitName    */ "Ohm",
        /* strSIUnitSymbol  */ QString(QChar(0x03A9)),
        /* strFormulaSymbol */ "R",
        /* bIsPowerRelated  */ false ),
    NanoOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    Ohm(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    MegaOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    nOhm(NanoOhm),
    uOhm(MicroOhm),
    mOhm(MilliOhm),
    kOhm(KiloOhm),
    MOhm(MegaOhm)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeResistance::CPhysSizeResistance( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSize(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Resistance",
        /* strSIUnitName    */ "Ohm",
        /* strSIUnitSymbol  */ QString(QChar(0x03A9)),
        /* strFormulaSymbol */ "R",
        /* bIsPowerRelated  */ false ),
    NanoOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    MicroOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    MilliOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    Ohm(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    KiloOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    MegaOhm(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    nOhm(NanoOhm),
    uOhm(MicroOhm),
    mOhm(MilliOhm),
    kOhm(KiloOhm),
    MOhm(MegaOhm)
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

    if( i_pPhysUnitRef != nullptr && i_pPhysUnitRef != &Ohm )
    {
        fValRef = Ohm.convertValue(fValRef, i_pPhysUnitRef);
    }
    return fValRef;
}
