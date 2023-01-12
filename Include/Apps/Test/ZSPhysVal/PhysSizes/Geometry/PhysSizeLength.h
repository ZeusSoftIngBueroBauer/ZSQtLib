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

#ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_Length_h
#define ZSApps_TestPhysVal_PhysSizes_Geometry_Length_h

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
class CPhysSizeLength : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeLength(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeLength(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeLength(CPhysSizeLength&& i_other) = delete;
    CPhysSizeLength(CPhysSizeLength& i_other) = delete;
    CPhysSizeLength(const CPhysSizeLength& i_other) = delete;
    virtual ~CPhysSizeLength();
public: // operators
    CPhysSizeLength& operator=(CPhysSizeLength& i_other) = delete;
    CPhysSizeLength& operator=(const CPhysSizeLength& i_other) = delete;
    CPhysSizeLength& operator=(CPhysSizeLength&& i_other) = delete;
private: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryPicoMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryNanoMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMicroMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMilliMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryCentiMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryDeziMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKiloMeter;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryInch;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryFoot;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryYard;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMile;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryNauticalMile;
public: // instance members
    ZS::PhysVal::CPhysUnit PicoMeter;
    ZS::PhysVal::CPhysUnit NanoMeter;
    ZS::PhysVal::CPhysUnit MicroMeter;
    ZS::PhysVal::CPhysUnit MilliMeter;
    ZS::PhysVal::CPhysUnit CentiMeter;
    ZS::PhysVal::CPhysUnit DeziMeter;
    ZS::PhysVal::CPhysUnit Meter;
    ZS::PhysVal::CPhysUnit KiloMeter;
    ZS::PhysVal::CPhysUnit Inch;
    ZS::PhysVal::CPhysUnit Foot;
    ZS::PhysVal::CPhysUnit Yard;
    ZS::PhysVal::CPhysUnit Mile;
    ZS::PhysVal::CPhysUnit NauticalMile;
    ZS::PhysVal::CPhysUnit pm;
    ZS::PhysVal::CPhysUnit nm;
    ZS::PhysVal::CPhysUnit um;
    ZS::PhysVal::CPhysUnit mm;
    ZS::PhysVal::CPhysUnit cm;
    ZS::PhysVal::CPhysUnit dm;
    ZS::PhysVal::CPhysUnit m;
    ZS::PhysVal::CPhysUnit km;
    ZS::PhysVal::CPhysUnit ft;
    ZS::PhysVal::CPhysUnit yd;

}; // CPhysSizeLength

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_Length_h
