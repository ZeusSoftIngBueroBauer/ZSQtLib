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

#ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Current_h
#define ZSApps_TestPhysVal_PhysSizes_Electricity_Current_h

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
const double c_fRefValCurrent_1A = 1.0;

//******************************************************************************
class CPhysSizeCurrent : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeCurrent(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeCurrent(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeCurrent(CPhysSizeCurrent&& i_other) = delete;
    CPhysSizeCurrent(CPhysSizeCurrent& i_other) = delete;
    CPhysSizeCurrent(const CPhysSizeCurrent& i_other) = delete;
    virtual ~CPhysSizeCurrent();
public: // operators
    CPhysSizeCurrent& operator=(CPhysSizeCurrent& i_other) = delete;
    CPhysSizeCurrent& operator=(const CPhysSizeCurrent& i_other) = delete;
    CPhysSizeCurrent& operator=(CPhysSizeCurrent&& i_other) = delete;
public: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryPicoAmpere;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryNanoAmpere;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMicroAmpere;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryMilliAmpere;
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryAmpere;
public: // instance members
    ZS::PhysVal::CPhysUnit PicoAmpere;
    ZS::PhysVal::CPhysUnit NanoAmpere;
    ZS::PhysVal::CPhysUnit MicroAmpere;
    ZS::PhysVal::CPhysUnit MilliAmpere;
    ZS::PhysVal::CPhysUnit Ampere;
    ZS::PhysVal::CPhysUnit pA;
    ZS::PhysVal::CPhysUnit nA;
    ZS::PhysVal::CPhysUnit uA;
    ZS::PhysVal::CPhysUnit mA;
    ZS::PhysVal::CPhysUnit A;

}; // class CPhysSizeCurrent

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Electricity_Current_h
