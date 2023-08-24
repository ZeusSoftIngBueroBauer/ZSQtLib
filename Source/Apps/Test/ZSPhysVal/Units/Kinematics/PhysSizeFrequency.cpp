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

#include "Units/Kinematics/PhysSizeFrequency.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeFrequency : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeFrequency::CPhysSizeFrequency( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Frequency",
        /* strSIUnitName    */ "Hertz",
        /* strSIUnitSymbol  */ "Hz",
        /* strFormulaSymbol */ "f",
        /* bIsPowerRelated  */ false ),
    m_treeEntryHertz(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    m_treeEntryKiloHertz(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixKilo ),
    m_treeEntryMegaHertz(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMega ),
    m_treeEntryGigaHertz(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixGiga ),
    Hertz(m_treeEntryHertz),
    KiloHertz(m_treeEntryKiloHertz),
    MegaHertz(m_treeEntryMegaHertz),
    GigaHertz(m_treeEntryGigaHertz),
    Hz(m_treeEntryHertz),
    kHz(m_treeEntryKiloHertz),
    MHz(m_treeEntryMegaHertz),
    GHz(m_treeEntryGigaHertz)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

    // To allow "short" unit strings like "Frequency.Hz" we add a shortcut to this phyiscal size.
    i_pParentBranch->tree()->addShortcut(this, "Frequency");

    // To allow "short" unit strings like "Hz" we add shortcuts to each unit.
    i_pParentBranch->tree()->addShortcut(&m_treeEntryHertz, "Hz");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryKiloHertz, "kHz");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryMegaHertz, "MHz");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryGigaHertz, "GHz");

} // ctor

//------------------------------------------------------------------------------
CPhysSizeFrequency::~CPhysSizeFrequency()
//------------------------------------------------------------------------------
{
} // dtor
