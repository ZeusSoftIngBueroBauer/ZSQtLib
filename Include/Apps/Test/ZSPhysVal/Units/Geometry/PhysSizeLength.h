/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSApps_TestPhysVal_Units_Geometry_Length_h
#define ZSApps_TestPhysVal_Units_Geometry_Length_h

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
//******************************************************************************
class CPhysSizeLength : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeLength(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizeLength();
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryPicoMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryCentiMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryDeziMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloMeter;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryInch;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryFoot;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryYard;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMile;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNauticalMile;
public: // instance members
    ZS::PhysVal::CUnit PicoMeter;
    ZS::PhysVal::CUnit NanoMeter;
    ZS::PhysVal::CUnit MicroMeter;
    ZS::PhysVal::CUnit MilliMeter;
    ZS::PhysVal::CUnit CentiMeter;
    ZS::PhysVal::CUnit DeziMeter;
    ZS::PhysVal::CUnit Meter;
    ZS::PhysVal::CUnit KiloMeter;
    ZS::PhysVal::CUnit Inch;
    ZS::PhysVal::CUnit Foot;
    ZS::PhysVal::CUnit Yard;
    ZS::PhysVal::CUnit Mile;
    ZS::PhysVal::CUnit NauticalMile;
    ZS::PhysVal::CUnit pm;
    ZS::PhysVal::CUnit nm;
    ZS::PhysVal::CUnit um;
    ZS::PhysVal::CUnit mm;
    ZS::PhysVal::CUnit cm;
    ZS::PhysVal::CUnit dm;
    ZS::PhysVal::CUnit m;
    ZS::PhysVal::CUnit km;
    ZS::PhysVal::CUnit ft;
    ZS::PhysVal::CUnit yd;

}; // CPhysSizeLength

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Geometry_Length_h
