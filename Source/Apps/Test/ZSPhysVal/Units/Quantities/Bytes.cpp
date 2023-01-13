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

#include "Units/Quantities/Bytes.h"
#include "ZSPhysVal/ZSPhysTreeEntryGrpScienceField.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


/*******************************************************************************
class CUnitsQuantitiesBytes : public CUnitsTreeEntryGrpBase
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
CUnitsQuantitiesBytes::CUnitsQuantitiesBytes(CIdxTreeEntry* i_pParentBranch) :
//------------------------------------------------------------------------------
    CUnitsTreeEntryGrpBase(i_pParentBranch, EUnitClassType::Quantity, "Bytes"),
    m_treeEntryByte(
        /* pUnitGrp  */ this,
        /* strName   */ "Byte",
        /* strSymbol */ "B",
        /* fFactor   */ 1.0 ),
    m_treeEntryKiloByte(
        /* pUnitGrp  */ this,
        /* strName   */ "KiloByte",
        /* strSymbol */ "kB",
        /* fFactor   */ 1024.0 ),
    m_treeEntryMegaByte(
        /* pUnitGrp  */ this,
        /* strName   */ "MegaByte",
        /* strSymbol */ "MB",
        /* fFactor   */ 1024.0*1024.0 ),
    m_treeEntryGigaByte(
        /* pUnitGrp  */ this,
        /* strName   */ "GigaByte",
        /* strSymbol */ "GB",
        /* fFactor   */ 1024.0*1024.0*1024.0 ),
    Byte(m_treeEntryByte),
    KiloByte(m_treeEntryKiloByte),
    MegaByte(m_treeEntryMegaByte),
    GigaByte(m_treeEntryGigaByte),
    kB(m_treeEntryKiloByte),
    MB(m_treeEntryMegaByte),
    GB(m_treeEntryGigaByte)
{
    // To allow "short" unit strings like "Bytes.kB" we add a shortcut to this phyiscal size.
    i_pParentBranch->tree()->addShortcut(this, "Bytes");

    // To allow "short" unit strings like "MB" we add shortcuts to each unit.
    i_pParentBranch->tree()->addShortcut(&m_treeEntryKiloByte, "kB");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryMegaByte, "MB");
    i_pParentBranch->tree()->addShortcut(&m_treeEntryGigaByte, "GB");
}

//------------------------------------------------------------------------------
CUnitsQuantitiesBytes::~CUnitsQuantitiesBytes()
//------------------------------------------------------------------------------
{
} // dtor
