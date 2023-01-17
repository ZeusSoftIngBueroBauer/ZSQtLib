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

#include "Units/Units.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal;


#ifdef ZSSys_MemLeakDump_h
#pragma push_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
#pragma warning( disable : 4005 )
#define _ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE 0
#endif


CIdxTreeUnitsPhysValTest ZS::Apps::Test::PhysVal::IdxTreeUnits;

CUnitsQuantities ZS::Apps::Test::PhysVal::Quantities(IdxTreeUnits.root());
CUnitsRatios ZS::Apps::Test::PhysVal::Ratio(IdxTreeUnits.root());
CPhysScienceFieldElectricity ZS::Apps::Test::PhysVal::Electricity(IdxTreeUnits.scienceFieldsTreeEntry());
CPhysScienceFieldGeometry ZS::Apps::Test::PhysVal::Geometry(IdxTreeUnits.scienceFieldsTreeEntry());
CPhysScienceFieldKinematics ZS::Apps::Test::PhysVal::Kinematics(IdxTreeUnits.scienceFieldsTreeEntry());


/*******************************************************************************
class CIdxTreeUnitsPhysValTest : public CIdxTreeUnits
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
/*! @brief 

*/
CIdxTreeUnitsPhysValTest::CIdxTreeUnitsPhysValTest() :
//------------------------------------------------------------------------------
    CIdxTreeUnits(new CIdxTreeEntry(EIdxTreeEntryType::Root, "Units")),
    m_treeEntryScienceFields(EIdxTreeEntryType::Branch, "Science Fields")
{
    add(&m_treeEntryScienceFields);
}

//------------------------------------------------------------------------------
CIdxTreeUnitsPhysValTest::~CIdxTreeUnitsPhysValTest()
//------------------------------------------------------------------------------
{
}

/*==============================================================================
public: // instance methods
==============================================================================*/

//------------------------------------------------------------------------------
CIdxTreeEntry* CIdxTreeUnitsPhysValTest::scienceFieldsTreeEntry()
//------------------------------------------------------------------------------
{
    return &m_treeEntryScienceFields;
}

#ifdef ZSSys_MemLeakDump_h
#pragma warning( default : 4005 )
#pragma pop_macro("_ZSSYS_DBGNEW_CLIENT_BLOCK_SUBTYPE")
#endif
