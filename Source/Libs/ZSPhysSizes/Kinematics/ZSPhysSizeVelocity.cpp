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

#include "ZSPhysSizes/Kinematics/ZSPhysSizeVelocity.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Kinematics;


/*******************************************************************************
class CPhysSizeVelocity : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeVelocity::CPhysSizeVelocity() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldKinematics,
        /* strName          */ "Velocity",
        /* strSIUnitName    */ "Meter Per Second",
        /* strSIUnitSymbol  */ "m/s",
        /* strFormulaSymbol */ "v",
        /* bIsPowerRelated  */ false ),
    m_physUnitMetersPerSecond(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_physUnitKilometersPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Kilometer Per Hour",
        /* strSymbol      */ "km/h",
        /* fMFromSI       */ 3.6 ),
    m_physUnitMilesPerHour(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ false,
        /* strName        */ "Miles Per Hour",
        /* strSymbol      */ "mi/h",
        /* fMFromSI       */ 3600.0/1.609344e3 )
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
