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

#ifndef ZSApps_TestPhysVal_Units_Ratios_h
#define ZSApps_TestPhysVal_Units_Ratios_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpBase.h"
#include "ZSPhysVal/ZSPhysTreeEntryUnitRatio.h"
#include "ZSPhysVal/ZSPhysUnit.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreeUnits;
}
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CUnitsRatios : public ZS::PhysVal::CUnitsTreeEntryGrpBase
//******************************************************************************
{
public: // ctors and dtor
    CUnitsRatios(ZS::System::CIdxTreeEntry* i_pParentBranch);
    virtual ~CUnitsRatios();
public: // instance members
    ZS::PhysVal::CUnitsTreeEntryUnitRatio m_treeEntryRatioPerOne;
    ZS::PhysVal::CUnitsTreeEntryUnitRatio m_treeEntryRatioPerCent;
    ZS::PhysVal::CUnitsTreeEntryUnitRatio m_treeEntryRatioPerMille;
    ZS::PhysVal::CUnitsTreeEntryUnitRatio m_treeEntryRatioDezibel;
public: // instance members
    ZS::PhysVal::CUnit PerOne;
    ZS::PhysVal::CUnit PerCent;
    ZS::PhysVal::CUnit PerMille;
    ZS::PhysVal::CUnit Dezibel;
    ZS::PhysVal::CUnit db;

}; // class CUnitsRatios

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Ratios_h
