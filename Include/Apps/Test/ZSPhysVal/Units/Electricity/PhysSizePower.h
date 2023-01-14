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

#ifndef ZSApps_TestPhysVal_Units_Electricity_Power_h
#define ZSApps_TestPhysVal_Units_Electricity_Power_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnit.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CPhysSizePower : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizePower(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizePower();
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPicoWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMegaWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryGigaWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBMilliWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBMicroWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBNanoWatt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBPicoWatt;
public: // instance members
    ZS::PhysVal::CUnit PicoWatt;
    ZS::PhysVal::CUnit NanoWatt;
    ZS::PhysVal::CUnit MicroWatt;
    ZS::PhysVal::CUnit MilliWatt;
    ZS::PhysVal::CUnit Watt;
    ZS::PhysVal::CUnit KiloWatt;
    ZS::PhysVal::CUnit MegaWatt;
    ZS::PhysVal::CUnit GigaWatt;
    ZS::PhysVal::CUnit dBWatt;
    ZS::PhysVal::CUnit dBMilliWatt;
    ZS::PhysVal::CUnit dBMicroWatt;
    ZS::PhysVal::CUnit dBNanoWatt;
    ZS::PhysVal::CUnit dBPicoWatt;
    ZS::PhysVal::CUnit pW;
    ZS::PhysVal::CUnit nW;
    ZS::PhysVal::CUnit uW;
    ZS::PhysVal::CUnit mW;
    ZS::PhysVal::CUnit W;
    ZS::PhysVal::CUnit kW;
    ZS::PhysVal::CUnit MW;
    ZS::PhysVal::CUnit GW;
    ZS::PhysVal::CUnit dBW;
    ZS::PhysVal::CUnit dBm;
    ZS::PhysVal::CUnit dBmW;
    ZS::PhysVal::CUnit dBuW;
    ZS::PhysVal::CUnit dBnW;
    ZS::PhysVal::CUnit dBpW;

}; // class CPhysSizePower

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Electricity_Power_h
