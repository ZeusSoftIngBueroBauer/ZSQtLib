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

#ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_h
#define ZSApps_TestPhysVal_PhysSizes_Electricity_h

#include "PhysSizes/Electricity/PhysSizeCurrent.h"
#include "PhysSizes/Electricity/PhysSizePower.h"
#include "PhysSizes/Electricity/PhysSizeResistance.h"
#include "PhysSizes/Electricity/PhysSizeVoltage.h"
#include "ZSPhysVal/ZSPhysScienceField.h"


/*******************************************************************************
interface description of exported methods
*******************************************************************************/

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
class CPhysScienceFieldElectricity : public ZS::PhysVal::CPhysScienceField
//******************************************************************************
{
public: // ctors and dtor
    CPhysScienceFieldElectricity(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysScienceFieldElectricity(CPhysScienceFieldElectricity&& i_other) = delete;
    CPhysScienceFieldElectricity(CPhysScienceFieldElectricity& i_other) = delete;
    CPhysScienceFieldElectricity(const CPhysScienceFieldElectricity& i_other) = delete;
    ~CPhysScienceFieldElectricity();
public: // operators
    CPhysScienceFieldElectricity& operator=(CPhysScienceFieldElectricity& i_other) = delete;
    CPhysScienceFieldElectricity& operator=(const CPhysScienceFieldElectricity& i_other) = delete;
    CPhysScienceFieldElectricity& operator=(CPhysScienceFieldElectricity&& i_other) = delete;
public: // instance members
    CPhysSizeCurrent Current;
    CPhysSizePower Power;
    CPhysSizeResistance Resistance;
    CPhysSizeVoltage Voltage;

}; // class CPhysScienceFieldElectricity

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_h
