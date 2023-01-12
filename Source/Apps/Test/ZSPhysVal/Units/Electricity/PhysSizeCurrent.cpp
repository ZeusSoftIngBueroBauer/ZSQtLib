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

#include "Units/Electricity/PhysSizeCurrent.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CPhysSizeCurrent : public CUnitsTreeEntryGrpPhysUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysSizeCurrent::CPhysSizeCurrent( CUnitsTreeEntryGrpScienceField* i_pParentBranch ) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpPhysUnits(
        /* pParentBranch    */ i_pParentBranch,
        /* strName          */ "Current",
        /* strSIUnitName    */ "Ampere",
        /* strSIUnitSymbol  */ "A",
        /* strFormulaSymbol */ "I",
        /* bIsPowerRelated  */ false ),
    m_treeEntryPicoAmpere(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixPico ),
    m_treeEntryNanoAmpere(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixNano ),
    m_treeEntryMicroAmpere(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMicro ),
    m_treeEntryMilliAmpere(
        /* pPhysSize */ this,
        /* strPrefix */ c_strPrefixMilli ),
    m_treeEntryAmpere(
        /* pPhysSize */ this,
        /* strPrefix */ "" ),
    PicoAmpere(m_treeEntryPicoAmpere),
    NanoAmpere(m_treeEntryNanoAmpere),
    MicroAmpere(m_treeEntryMicroAmpere),
    MilliAmpere(m_treeEntryMilliAmpere),
    Ampere(m_treeEntryAmpere),
    pA(m_treeEntryPicoAmpere),
    nA(m_treeEntryNanoAmpere),
    uA(m_treeEntryMicroAmpere),
    mA(m_treeEntryMilliAmpere),
    A(m_treeEntryAmpere)
{
    // Call function of base class CPhysSize to initialize the physical size together
    // with its units (e.g. to create the field with internal conversion routines
    // and to create the chained list of Lower/Higher units).
    initialize(true);

} // ctor

//------------------------------------------------------------------------------
CPhysSizeCurrent::~CPhysSizeCurrent()
//------------------------------------------------------------------------------
{
} // dtor
