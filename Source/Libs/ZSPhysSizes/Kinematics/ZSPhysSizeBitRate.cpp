/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#include "ZSPhysSizes/Kinematics/ZSPhysSizeBitRate.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System::Math;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Kinematics;


/*******************************************************************************
class CPhysSizeBitRate : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeBitRate::CPhysSizeBitRate() :
//------------------------------------------------------------------------------
    CPhysSize(
        /* scienceField     */ EPhysScienceFieldKinematics,
        /* strName          */ "BitRate",
        /* strSIUnitName    */ "BitsPerSec",
        /* strSIUnitSymbol  */ "b/s",
        /* strFormulaSymbol */ "b/s",
        /* bIsPowerRelated  */ false ),
    m_physUnitBitsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ "BitsPerSec",
        /* strSymbol          */ "b/s",
        /* fMFromBaseOrRefVal */ 1.0 ),
    m_physUnitKiloBitsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ c_strKilo + "BitsPerSec",
        /* strSymbol          */ c_strPrefixKilo + "b/s",
        /* fMFromBaseOrRefVal */ 1.0e-3 ),
    m_physUnitSymbolsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ "SymbolsPerSec",
        /* strSymbol          */ "Symb/s",
        /* fMFromBaseOrRefVal */ 1.0 ),
    m_physUnitKiloSymbolsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ c_strKilo + "KiloSymbolsPerSec",
        /* strSymbol          */ c_strPrefixKilo + "Symb/s",
        /* fMFromBaseOrRefVal */ 1.0e-3 )
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeBitRate::~CPhysSizeBitRate()
//------------------------------------------------------------------------------
{
} // dtor
