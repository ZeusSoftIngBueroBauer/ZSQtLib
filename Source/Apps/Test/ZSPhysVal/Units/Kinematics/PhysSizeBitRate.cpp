/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include "Units/Kinematics/PhysSizeBitRate.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeBitRate : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeBitRate::CPhysSizeBitRate( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pIdxTree         */ i_pParentBranch,
        /* strName          */ "BitRate",
        /* strSIUnitName    */ "BitsPerSec",
        /* strSIUnitSymbol  */ "b/s",
        /* strFormulaSymbol */ "b/s",
        /* bIsPowerRelated  */ false ),
    m_treeEntryBitsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ "BitsPerSec",
        /* strSymbol          */ "b/s",
        /* fMFromBaseOrRefVal */ 1.0 ),
    m_treeEntryKiloBitsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ c_strKilo + QString("BitsPerSec"),
        /* strSymbol          */ c_strPrefixKilo + QString("b/s"),
        /* fMFromBaseOrRefVal */ 1.0e-3 ),
    m_treeEntrySymbolsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ "SymbolsPerSec",
        /* strSymbol          */ "Symb/s",
        /* fMFromBaseOrRefVal */ 1.0 ),
    m_treeEntryKiloSymbolsPerSec(
        /* pPhysSize          */ this,
        /* bIsLogarithmic     */ false,
        /* strName            */ c_strKilo + QString("SymbolsPerSec"),
        /* strSymbol          */ c_strPrefixKilo + QString("Symb/s"),
        /* fMFromBaseOrRefVal */ 1.0e-3 ),
    BitsPerSec(m_treeEntryBitsPerSec),
    KiloBitsPerSec(m_treeEntryKiloBitsPerSec),
    SymbolsPerSec(m_treeEntrySymbolsPerSec),
    KiloSymbolsPerSec(m_treeEntryKiloSymbolsPerSec),
    bps(m_treeEntryBitsPerSec),
    kbps(m_treeEntryKiloBitsPerSec),
    symps(m_treeEntrySymbolsPerSec),
    ksymps(m_treeEntryKiloSymbolsPerSec)
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
