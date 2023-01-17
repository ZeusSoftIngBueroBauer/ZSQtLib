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

#ifndef ZSApps_TestPhysVal_Units_Kinematics_Time_h
#define ZSApps_TestPhysVal_Units_Kinematics_Time_h

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
class CPhysSizeTime : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeTime(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizeTime();
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoSeconds;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroSeconds;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliSeconds;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntrySeconds;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMinutes;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryHours;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDays;
public: // instance members
    ZS::PhysVal::CUnit NanoSeconds;
    ZS::PhysVal::CUnit MicroSeconds;
    ZS::PhysVal::CUnit MilliSeconds;
    ZS::PhysVal::CUnit Seconds;
    ZS::PhysVal::CUnit Minutes;
    ZS::PhysVal::CUnit Hours;
    ZS::PhysVal::CUnit Days;
    ZS::PhysVal::CUnit ns;
    ZS::PhysVal::CUnit us;
    ZS::PhysVal::CUnit ms;
    ZS::PhysVal::CUnit m;
    ZS::PhysVal::CUnit s;
    ZS::PhysVal::CUnit h;
    ZS::PhysVal::CUnit d;

}; // class CPhysSizeTime

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Kinematics_Time_h
