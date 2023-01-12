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

#ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Velocity_h
#define ZSApps_TestPhysVal_PhysSizes_Kinematics_Velocity_h

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
class CPhysSizeVelocity : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeVelocity(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeVelocity(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeVelocity(CPhysSizeVelocity&& i_other) = delete;
    CPhysSizeVelocity(CPhysSizeVelocity& i_other) = delete;
    CPhysSizeVelocity(const CPhysSizeVelocity& i_other) = delete;
    virtual ~CPhysSizeVelocity();
public: // operators
    CPhysSizeVelocity& operator=(CPhysSizeVelocity& i_other) = delete;
    CPhysSizeVelocity& operator=(const CPhysSizeVelocity& i_other) = delete;
    CPhysSizeVelocity& operator=(CPhysSizeVelocity&& i_other) = delete;
public: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMetersPerSecond;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKilometersPerHour;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMilesPerHour;
public: // instance members
    ZS::PhysVal::CPhysUnit MetersPerSecond;
    ZS::PhysVal::CPhysUnit KilometersPerHour;
    ZS::PhysVal::CPhysUnit MilesPerHour;
    ZS::PhysVal::CPhysUnit mps;
    ZS::PhysVal::CPhysUnit kmph;
    ZS::PhysVal::CPhysUnit milesph;

}; // class CPhysSizeVelocity

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Velocity_h
