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

#ifndef ZSApps_TestPhysVal_Units_Electricity_Resistance_h
#define ZSApps_TestPhysVal_Units_Electricity_Resistance_h

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
const double c_fRefValResistance_50Ohm = 50.0;
const double c_fRefValResistance_10log50Ohm = 10.0*log10(50.0);

//******************************************************************************
class CPhysSizeResistance : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeResistance(ZS::PhysVal::CUnitsTreeEntryGrpScienceField* i_pParentBranch);
    virtual ~CPhysSizeResistance();
public: // overridables of base class CPhysSize
    virtual double getRefVal( const ZS::PhysVal::CUnit& i_unitRef ) const;
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryNanoOhm;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMicroOhm;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMilliOhm;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryOhm;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloOhm;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMegaOhm;
public: // class members
    static const QString c_strSymbolOhm;
public: // instance members
    ZS::PhysVal::CUnit NanoOhm;
    ZS::PhysVal::CUnit MicroOhm;
    ZS::PhysVal::CUnit MilliOhm;
    ZS::PhysVal::CUnit Ohm;
    ZS::PhysVal::CUnit KiloOhm;
    ZS::PhysVal::CUnit MegaOhm;
    ZS::PhysVal::CUnit nOhm;
    ZS::PhysVal::CUnit uOhm;
    ZS::PhysVal::CUnit mOhm;
    ZS::PhysVal::CUnit kOhm;
    ZS::PhysVal::CUnit MOhm;

}; // class CPhysSizeResistance

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_Electricity_Resistance_h
