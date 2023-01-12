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

#ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Frequency_h
#define ZSApps_TestPhysVal_PhysSizes_Kinematics_Frequency_h

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
const double c_fRefValSampleRate_1000Hz = 1000.0;

//******************************************************************************
class CPhysSizeFrequency : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeFrequency(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeFrequency(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeFrequency(CPhysSizeFrequency&& i_other) = delete;
    CPhysSizeFrequency(CPhysSizeFrequency& i_other) = delete;
    CPhysSizeFrequency(const CPhysSizeFrequency& i_other) = delete;
    virtual ~CPhysSizeFrequency();
public: // operators
    CPhysSizeFrequency& operator=(CPhysSizeFrequency& i_other) = delete;
    CPhysSizeFrequency& operator=(const CPhysSizeFrequency& i_other) = delete;
    CPhysSizeFrequency& operator=(CPhysSizeFrequency&& i_other) = delete;
private: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryHertz;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKiloHertz;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMegaHertz;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryGigaHertz;
public: // instance members
    ZS::PhysVal::CPhysUnit Hertz;
    ZS::PhysVal::CPhysUnit KiloHertz;
    ZS::PhysVal::CPhysUnit MegaHertz;
    ZS::PhysVal::CPhysUnit GigaHertz;
    ZS::PhysVal::CPhysUnit Hz;
    ZS::PhysVal::CPhysUnit kHz;
    ZS::PhysVal::CPhysUnit MHz;
    ZS::PhysVal::CPhysUnit GHz;

}; // class CPhysSizeFrequency

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_Frequency_h
