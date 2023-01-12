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

#ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_h
#define ZSApps_TestPhysVal_PhysSizes_Kinematics_h

#include "PhysSizes/Kinematics/PhysSizeBitRate.h"
#include "PhysSizes/Kinematics/PhysSizeFrequency.h"
#include "PhysSizes/Kinematics/PhysSizeSamples.h"
#include "PhysSizes/Kinematics/PhysSizeTime.h"
#include "PhysSizes/Kinematics/PhysSizeVelocity.h"
#include "ZSPhysVal/ZSPhysScienceField.h"

namespace ZS
{
namespace PhysVal
{
class CIdxTreePhysSizes;
}
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CPhysScienceFieldKinematics : public ZS::PhysVal::CPhysScienceFieldTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysScienceFieldKinematics(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysScienceFieldKinematics(CPhysScienceFieldKinematics&& i_other) = delete;
    CPhysScienceFieldKinematics(CPhysScienceFieldKinematics& i_other) = delete;
    CPhysScienceFieldKinematics(const CPhysScienceFieldKinematics& i_other) = delete;
    ~CPhysScienceFieldKinematics();
public: // operators
    CPhysScienceFieldKinematics& operator=(CPhysScienceFieldKinematics& i_other) = delete;
    CPhysScienceFieldKinematics& operator=(const CPhysScienceFieldKinematics& i_other) = delete;
    CPhysScienceFieldKinematics& operator=(CPhysScienceFieldKinematics&& i_other) = delete;
private: // instance members
    CPhysSizeBitRate m_treeEntryBitRate;
    CPhysSizeFrequency m_treeEntryFrequency;
    CPhysSizeSamples m_treeEntrySamples;
    CPhysSizeTime m_treeEntryTime;
    CPhysSizeVelocity m_treeEntryVelocity;
public: // instance members
    ZS::PhysVal::CPhysSize BitRate;
    ZS::PhysVal::CPhysSize Frequency;
    ZS::PhysVal::CPhysSize Samples;
    ZS::PhysVal::CPhysSize Time;
    ZS::PhysVal::CPhysSize Velocity;

}; // class CPhysScienceFieldKinematics

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_h
