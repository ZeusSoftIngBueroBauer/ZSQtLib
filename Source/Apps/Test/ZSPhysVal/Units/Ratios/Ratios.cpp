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

#include "Units/Ratios/Ratios.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CUnitsRatios
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
CUnitsRatios::CUnitsRatios(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(i_pParentBranch, EUnitClassType::Ratio, "Ratios"),
    m_treeEntryRatioPerOne(
        /* pUnitGrp  */ this,
        /* strName   */ "Per One",
        /* strSymbol */ "",
        /* fFactor   */ 1.0 ),
    m_treeEntryRatioPerCent(
        /* pUnitGrp  */ this,
        /* strName   */ "Per Cent",
        /* strSymbol */ "%",
        /* fFactor   */ c_fFactorCenti ),
    m_treeEntryRatioPerMille(
        /* pUnitGrp  */ this,
        /* strName   */ "Per Mille",
        /* strSymbol */ "%%",
        /* fFactor   */ c_fFactorMilli ),
    m_treeEntryRatioDezibel(
        /* pUnitGrp  */ this,
        /* strName   */ "Dezibel",
        /* strSymbol */ "dB",
        /* fFactor   */ c_fFactor1 ),
    PerOne(m_treeEntryRatioPerOne),
    PerCent(m_treeEntryRatioPerCent),
    PerMille(m_treeEntryRatioPerMille),
    Dezibel(m_treeEntryRatioDezibel),
    dB(m_treeEntryRatioDezibel)
{
}

//------------------------------------------------------------------------------
CUnitsRatios::~CUnitsRatios()
//------------------------------------------------------------------------------
{
} // dtor
