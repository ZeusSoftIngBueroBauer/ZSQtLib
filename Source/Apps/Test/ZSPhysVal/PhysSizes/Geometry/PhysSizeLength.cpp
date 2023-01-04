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

#if 0

using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Geometry;


/*******************************************************************************
class CPhysSizeLength : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeLength::CPhysSizeLength() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldGeometry,
        /* strName          */ "Length",
        /* strSIUnitName    */ "Meter",
        /* strSIUnitSymbol  */ "m",
        /* strFormulaSymbol */ "l",
        /* bIsPowerRelated  */ false ),
    m_physUnitPicoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_physUnitNanoMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_physUnitMicroMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_physUnitMilliMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_physUnitCentiMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixCenti ),
    m_physUnitDeziMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixDezi ),
    m_physUnitMeter(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKiloMeter(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_physUnitInch(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Inch",
        /* strSymbol      */ "in",
        /* fMFromSI       */ 1.0/0.0254 ),
    m_physUnitFoot(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Foot",
        /* strSymbol      */ "ft",
        /* fMFromSI       */ 1.0/(12.0*0.0254) ),
    m_physUnitYard(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Yard",
        /* strSymbol      */ "yd",
        /* fMFromSI       */ 1.0/(36.0*0.0254) ),
    m_physUnitMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Mile",
        /* strSymbol      */ "mi",
        /* fMFromSI       */ 1.0/1609.344 ),
    m_physUnitNauticalMile(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Nautical Mile",
        /* strSymbol      */ "sm",
        /* fMFromSI       */ 1.0/1852.0 )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines).
    // Here we don't let the physical size create the chained list of Lower/Higher units
    // for the "findBestUnit" functionality as "Inch", "Feet", "DeziMeter", "CentiMeter"
    // etc. don't belong to those chained list.
    initialize(false);

    // Link the units to a chained list for the "findBestUnit" functionality:
    //======================================================================

    m_physUnitPicoMeter. setNextLowerHigherUnits( nullptr,               &m_physUnitNanoMeter  );
    m_physUnitNanoMeter. setNextLowerHigherUnits( &m_physUnitPicoMeter,  &m_physUnitMicroMeter );
    m_physUnitMicroMeter.setNextLowerHigherUnits( &m_physUnitNanoMeter,  &m_physUnitMilliMeter );
    m_physUnitMilliMeter.setNextLowerHigherUnits( &m_physUnitMicroMeter, &m_physUnitMeter      );
    m_physUnitMeter.     setNextLowerHigherUnits( &m_physUnitMilliMeter, &m_physUnitKiloMeter  );
    m_physUnitKiloMeter. setNextLowerHigherUnits( &m_physUnitMeter,      nullptr               );

} // ctor

//------------------------------------------------------------------------------
CPhysSizeLength::~CPhysSizeLength()
//------------------------------------------------------------------------------
{
} // dtor

#endif
