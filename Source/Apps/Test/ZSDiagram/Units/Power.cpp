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

#include "Units/Power.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::Diagram;


/*******************************************************************************
class CPhysSizePower : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizePower::CPhysSizePower( CIdxTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Power",
        /* strSIUnitName    */ "Watt",
        /* strSIUnitSymbol  */ "W",
        /* strFormulaSymbol */ "P",
        /* bIsPowerRelated  */ true ),
    m_treeEntryWatt(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntrydBMilliWatt(
        /* pPhysSize      */ this,
        /* bIsLogarithmic */ true,
        /* strName        */ "dBMilliWatt",
        /* strSymbol      */ "dB" + QString(c_strPrefixMilli),
        /* fRefVal        */ c_fFactorMilli ),
    Watt(m_treeEntryWatt),
    dBMilliWatt(m_treeEntrydBMilliWatt),
    W(m_treeEntryWatt),
    dBm(m_treeEntrydBMilliWatt)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

    // To allow "short" unit strings like "Power.mW" we add a shortcut to this phyiscal size.
    i_pParentBranch->tree()->addShortcut(this, "Power");

} // ctor

//------------------------------------------------------------------------------
CPhysSizePower::~CPhysSizePower()
//------------------------------------------------------------------------------
{
} // dtor
