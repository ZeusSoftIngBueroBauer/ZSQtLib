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

#ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Voltage_h
#define ZSApps_TestPhysVal_PhysSizes_Electricity_Voltage_h

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
const double c_fRefValdBu_0775V = sqrt(0.6);

//******************************************************************************
class CPhysSizeVoltage : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeVoltage(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeVoltage(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeVoltage(CPhysSizeVoltage&& i_other) = delete;
    CPhysSizeVoltage(CPhysSizeVoltage& i_other) = delete;
    CPhysSizeVoltage(const CPhysSizeVoltage& i_other) = delete;
    virtual ~CPhysSizeVoltage();
public: // operators
    CPhysSizeVoltage& operator=(CPhysSizeVoltage& i_other) = delete;
    CPhysSizeVoltage& operator=(const CPhysSizeVoltage& i_other) = delete;
    CPhysSizeVoltage& operator=(CPhysSizeVoltage&& i_other) = delete;
private: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryPicoVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryNanoVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMicroVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMilliVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryKiloVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMegaVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBu;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBMilliVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBMicroVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBNanoVolt;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntrydBPicoVolt;
public: // instance members
    ZS::PhysVal::CPhysUnit PicoVolt;
    ZS::PhysVal::CPhysUnit NanoVolt;
    ZS::PhysVal::CPhysUnit MicroVolt;
    ZS::PhysVal::CPhysUnit MilliVolt;
    ZS::PhysVal::CPhysUnit Volt;
    ZS::PhysVal::CPhysUnit KiloVolt;
    ZS::PhysVal::CPhysUnit MegaVolt;
    ZS::PhysVal::CPhysUnit dBVolt;
    ZS::PhysVal::CPhysUnit dBu;
    ZS::PhysVal::CPhysUnit dBMilliVolt;
    ZS::PhysVal::CPhysUnit dBMicroVolt;
    ZS::PhysVal::CPhysUnit dBNanoVolt;
    ZS::PhysVal::CPhysUnit dBPicoVolt;
    ZS::PhysVal::CPhysUnit pV;
    ZS::PhysVal::CPhysUnit nV;
    ZS::PhysVal::CPhysUnit uV;
    ZS::PhysVal::CPhysUnit mV;
    ZS::PhysVal::CPhysUnit V;
    ZS::PhysVal::CPhysUnit kV;
    ZS::PhysVal::CPhysUnit MV;
    ZS::PhysVal::CPhysUnit dBV;
    ZS::PhysVal::CPhysUnit dBmV;
    ZS::PhysVal::CPhysUnit dBuV;
    ZS::PhysVal::CPhysUnit dBnV;
    ZS::PhysVal::CPhysUnit dBpV;

}; // class CPhysSizeVoltage

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Voltage_h
