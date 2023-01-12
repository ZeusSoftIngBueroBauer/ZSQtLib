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

#ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_BitRate_h
#define ZSApps_TestPhysVal_PhysSizes_Kinematics_BitRate_h

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
class CPhysSizeBitRate : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeBitRate(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeBitRate(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeBitRate(CPhysSizeBitRate&& i_other) = delete;
    CPhysSizeBitRate(CPhysSizeBitRate& i_other) = delete;
    CPhysSizeBitRate(const CPhysSizeBitRate& i_other) = delete;
    virtual ~CPhysSizeBitRate();
public: // operators
    CPhysSizeBitRate& operator=(CPhysSizeBitRate& i_other) = delete;
    CPhysSizeBitRate& operator=(const CPhysSizeBitRate& i_other) = delete;
    CPhysSizeBitRate& operator=(CPhysSizeBitRate&& i_other) = delete;
private: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryBitsPerSec;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKiloBitsPerSec;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrySymbolsPerSec;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKiloSymbolsPerSec;
public: // instance members
    ZS::PhysVal::CPhysUnit BitsPerSec;
    ZS::PhysVal::CPhysUnit KiloBitsPerSec;
    ZS::PhysVal::CPhysUnit SymbolsPerSec;
    ZS::PhysVal::CPhysUnit KiloSymbolsPerSec;
    ZS::PhysVal::CPhysUnit bps;
    ZS::PhysVal::CPhysUnit kbps;
    ZS::PhysVal::CPhysUnit symps;
    ZS::PhysVal::CPhysUnit ksymps;

}; // class CPhysSizeBitRate

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Kinematics_BitRate_h
