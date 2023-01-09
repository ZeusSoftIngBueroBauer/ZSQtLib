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

#ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Resistance_h
#define ZSApps_TestPhysVal_PhysSizes_Electricity_Resistance_h

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
const double c_fRefValResistance_50Ohm = 50.0;
const double c_fRefValResistance_10log50Ohm = 10.0*log10(50.0);

//******************************************************************************
class CPhysSizeResistance : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeResistance( ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree );
    CPhysSizeResistance(ZS::System::CIdxTreeEntry* i_pParentBranch);
    CPhysSizeResistance(CPhysSizeResistance&& i_other) = delete;
    CPhysSizeResistance(CPhysSizeResistance& i_other) = delete;
    CPhysSizeResistance(const CPhysSizeResistance& i_other) = delete;
    virtual ~CPhysSizeResistance();
public: // operators
    CPhysSizeResistance& operator=(CPhysSizeResistance& i_other) = delete;
    CPhysSizeResistance& operator=(const CPhysSizeResistance& i_other) = delete;
    CPhysSizeResistance& operator=(CPhysSizeResistance&& i_other) = delete;
public: // overridables of base class CPhysSize
    virtual double getRefVal( ZS::PhysVal::CPhysUnit* i_pPhysUnitRef ) const;
public: // class members
    static const QString c_strSymbolOhm;
public: // instance members
    ZS::PhysVal::CPhysUnit NanoOhm;
    ZS::PhysVal::CPhysUnit MicroOhm;
    ZS::PhysVal::CPhysUnit MilliOhm;
    ZS::PhysVal::CPhysUnit Ohm;
    ZS::PhysVal::CPhysUnit KiloOhm;
    ZS::PhysVal::CPhysUnit MegaOhm;
    ZS::PhysVal::CPhysUnit& nOhm;
    ZS::PhysVal::CPhysUnit& uOhm;
    ZS::PhysVal::CPhysUnit& mOhm;
    ZS::PhysVal::CPhysUnit& kOhm;
    ZS::PhysVal::CPhysUnit& MOhm;

}; // class CPhysSizeResistance

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Resistance_h
