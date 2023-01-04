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

#ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Time_h
#define ZSApps_TestPhysVal_PhysSizes_Kinematics_Time_h

#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CPhysSizeKinematicsTime : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeKinematicsTime(
        ZS::System::CIdxTree* i_pIdxTree,
        CIdxTreeEntry*        i_pParentBranch );
    ~CPhysSizeKinematicsTime();
public: // instance members
    ZS::PhysVal::CPhysUnit* NanoSeconds;
    ZS::PhysVal::CPhysUnit* MicroSeconds;
    ZS::PhysVal::CPhysUnit* MilliSeconds;
    ZS::PhysVal::CPhysUnit* Seconds;
    ZS::PhysVal::CPhysUnit* Minutes;
    ZS::PhysVal::CPhysUnit* Hours;
    ZS::PhysVal::CPhysUnit* Days;
    ZS::PhysVal::CPhysUnit* ns;
    ZS::PhysVal::CPhysUnit* us;
    ZS::PhysVal::CPhysUnit* ms;
    ZS::PhysVal::CPhysUnit* m;
    ZS::PhysVal::CPhysUnit* s;
    ZS::PhysVal::CPhysUnit* h;
    ZS::PhysVal::CPhysUnit* d;

}; // class CPhysSizeKinematicsTime

//namespace Kinematics
//{
////******************************************************************************
//class CPhysSizeTime : public ZS::PhysVal::CPhysSize
////******************************************************************************
//{
//public: // ctors and dtor
//    CPhysSizeTime( ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree );
//    ~CPhysSizeTime();
//public: // instance methods
//    ZS::PhysVal::CPhysUnit* NanoSeconds() { return &m_physUnitNanoSeconds; }
//    ZS::PhysVal::CPhysUnit* MicroSeconds() { return &m_physUnitMicroSeconds; }
//    ZS::PhysVal::CPhysUnit* MilliSeconds() { return &m_physUnitMilliSeconds; }
//    ZS::PhysVal::CPhysUnit* Seconds() { return &m_physUnitSeconds; }
//    ZS::PhysVal::CPhysUnit* Minutes() { return &m_physUnitMinutes; }
//    ZS::PhysVal::CPhysUnit* Hours() { return &m_physUnitHours; }
//    ZS::PhysVal::CPhysUnit* Days() { return &m_physUnitDays; }
//protected: // instance members
//    ZS::PhysVal::CPhysUnit m_physUnitNanoSeconds;
//    ZS::PhysVal::CPhysUnit m_physUnitMicroSeconds;
//    ZS::PhysVal::CPhysUnit m_physUnitMilliSeconds;
//    ZS::PhysVal::CPhysUnit m_physUnitSeconds;
//    ZS::PhysVal::CPhysUnit m_physUnitMinutes;
//    ZS::PhysVal::CPhysUnit m_physUnitHours;
//    ZS::PhysVal::CPhysUnit m_physUnitDays;
//
//}; // class CPhysSizeTime

//} // namespace Kinematics

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Time_h
