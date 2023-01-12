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

#include "PhysSizes/Kinematics/PhysSizeFrequency.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeFrequency : public CPhysSize
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeFrequency::CPhysSizeFrequency( CIdxTreePhysSizes* i_pIdxTree ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
        /* pIdxTree         */ i_pIdxTree,
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

} // ctor

//------------------------------------------------------------------------------
CPhysSizeFrequency::CPhysSizeFrequency( CPhysScienceFieldTreeEntry* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CPhysSizeTreeEntry(
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

} // ctor

//------------------------------------------------------------------------------
CPhysSizeFrequency::~CPhysSizeFrequency()
//------------------------------------------------------------------------------
{
} // dtor
