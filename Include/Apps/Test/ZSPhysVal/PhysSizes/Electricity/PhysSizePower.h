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

#ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Power_h
#define ZSApps_TestPhysVal_PhysSizes_Electricity_Power_h

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
class CPhysSizePower : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizePower( ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree );
    CPhysSizePower(ZS::System::CIdxTreeEntry* i_pParentBranch);
    CPhysSizePower(CPhysSizePower&& i_other) = delete;
    CPhysSizePower(CPhysSizePower& i_other) = delete;
    CPhysSizePower(const CPhysSizePower& i_other) = delete;
    virtual ~CPhysSizePower();
public: // operators
    CPhysSizePower& operator=(CPhysSizePower& i_other) = delete;
    CPhysSizePower& operator=(const CPhysSizePower& i_other) = delete;
    CPhysSizePower& operator=(CPhysSizePower&& i_other) = delete;
public: // instance members
    ZS::PhysVal::CPhysUnit PicoWatt;
    ZS::PhysVal::CPhysUnit NanoWatt;
    ZS::PhysVal::CPhysUnit MicroWatt;
    ZS::PhysVal::CPhysUnit MilliWatt;
    ZS::PhysVal::CPhysUnit Watt;
    ZS::PhysVal::CPhysUnit KiloWatt;
    ZS::PhysVal::CPhysUnit MegaWatt;
    ZS::PhysVal::CPhysUnit GigaWatt;
    ZS::PhysVal::CPhysUnit dBWatt;
    ZS::PhysVal::CPhysUnit dBMilliWatt;
    ZS::PhysVal::CPhysUnit dBMicroWatt;
    ZS::PhysVal::CPhysUnit dBNanoWatt;
    ZS::PhysVal::CPhysUnit dBPicoWatt;
    ZS::PhysVal::CPhysUnit& pW;
    ZS::PhysVal::CPhysUnit& nW;
    ZS::PhysVal::CPhysUnit& uW;
    ZS::PhysVal::CPhysUnit& mW;
    ZS::PhysVal::CPhysUnit& W;
    ZS::PhysVal::CPhysUnit& kW;
    ZS::PhysVal::CPhysUnit& MW;
    ZS::PhysVal::CPhysUnit& GW;
    ZS::PhysVal::CPhysUnit& dBW;
    ZS::PhysVal::CPhysUnit& dBm;
    ZS::PhysVal::CPhysUnit& dBmW;
    ZS::PhysVal::CPhysUnit& dBuW;
    ZS::PhysVal::CPhysUnit& dBnW;
    ZS::PhysVal::CPhysUnit& dBpW;

}; // class CPhysSizePower

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Power_h
