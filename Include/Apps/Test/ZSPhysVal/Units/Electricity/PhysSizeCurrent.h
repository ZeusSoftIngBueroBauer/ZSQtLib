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

#ifndef ZSApps_TestPhysVal_Units_Electricity_Current_h
#define ZSApps_TestPhysVal_Units_Electricity_Current_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnit.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
const double c_fRefValCurrent_1A = 1.0;

//******************************************************************************
class CPhysSizeCurrent : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeCurrent(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizeCurrent();
public: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPicoAmpere;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoAmpere;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroAmpere;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliAmpere;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryAmpere;
public: // instance members
    ZS::PhysVal::CUnit PicoAmpere;
    ZS::PhysVal::CUnit NanoAmpere;
    ZS::PhysVal::CUnit MicroAmpere;
    ZS::PhysVal::CUnit MilliAmpere;
    ZS::PhysVal::CUnit Ampere;
    ZS::PhysVal::CUnit pA;
    ZS::PhysVal::CUnit nA;
    ZS::PhysVal::CUnit uA;
    ZS::PhysVal::CUnit mA;
    ZS::PhysVal::CUnit A;

}; // class CPhysSizeCurrent

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Electricity_Current_h
