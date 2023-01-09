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
class CPhysSizeTime : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeTime(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeTime(ZS::System::CIdxTreeEntry* i_pParentBranch);
    CPhysSizeTime(CPhysSizeTime&& i_other) = delete;
    CPhysSizeTime(CPhysSizeTime& i_other) = delete;
    CPhysSizeTime(const CPhysSizeTime& i_other) = delete;
    virtual ~CPhysSizeTime();
public: // operators
    CPhysSizeTime& operator=(CPhysSizeTime& i_other) = delete;
    CPhysSizeTime& operator=(const CPhysSizeTime& i_other) = delete;
    CPhysSizeTime& operator=(CPhysSizeTime&& i_other) = delete;
public: // instance members
    ZS::PhysVal::CPhysUnit NanoSeconds;
    ZS::PhysVal::CPhysUnit MicroSeconds;
    ZS::PhysVal::CPhysUnit MilliSeconds;
    ZS::PhysVal::CPhysUnit Seconds;
    ZS::PhysVal::CPhysUnit Minutes;
    ZS::PhysVal::CPhysUnit Hours;
    ZS::PhysVal::CPhysUnit Days;
    ZS::PhysVal::CPhysUnit& ns;
    ZS::PhysVal::CPhysUnit& us;
    ZS::PhysVal::CPhysUnit& ms;
    ZS::PhysVal::CPhysUnit& m;
    ZS::PhysVal::CPhysUnit& s;
    ZS::PhysVal::CPhysUnit& h;
    ZS::PhysVal::CPhysUnit& d;

}; // class CPhysSizeTime

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Time_h
