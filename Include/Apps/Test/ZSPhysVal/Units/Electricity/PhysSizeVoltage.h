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

#ifndef ZSApps_TestPhysVal_Units_Electricity_Voltage_h
#define ZSApps_TestPhysVal_Units_Electricity_Voltage_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnit.h"

#include <math.h>

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
const double c_fRefValdBu_0775V = sqrt(0.6);

//******************************************************************************
class CPhysSizeVoltage : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeVoltage(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizeVoltage();
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPicoVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMegaVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBu;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBMilliVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBMicroVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBNanoVolt;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrydBPicoVolt;
public: // instance members
    ZS::PhysVal::CUnit PicoVolt;
    ZS::PhysVal::CUnit NanoVolt;
    ZS::PhysVal::CUnit MicroVolt;
    ZS::PhysVal::CUnit MilliVolt;
    ZS::PhysVal::CUnit Volt;
    ZS::PhysVal::CUnit KiloVolt;
    ZS::PhysVal::CUnit MegaVolt;
    ZS::PhysVal::CUnit dBVolt;
    ZS::PhysVal::CUnit dBu;
    ZS::PhysVal::CUnit dBMilliVolt;
    ZS::PhysVal::CUnit dBMicroVolt;
    ZS::PhysVal::CUnit dBNanoVolt;
    ZS::PhysVal::CUnit dBPicoVolt;
    ZS::PhysVal::CUnit pV;
    ZS::PhysVal::CUnit nV;
    ZS::PhysVal::CUnit uV;
    ZS::PhysVal::CUnit mV;
    ZS::PhysVal::CUnit V;
    ZS::PhysVal::CUnit kV;
    ZS::PhysVal::CUnit MV;
    ZS::PhysVal::CUnit dBV;
    ZS::PhysVal::CUnit dBmV;
    ZS::PhysVal::CUnit dBuV;
    ZS::PhysVal::CUnit dBnV;
    ZS::PhysVal::CUnit dBpV;

}; // class CPhysSizeVoltage

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Electricity_Voltage_h
