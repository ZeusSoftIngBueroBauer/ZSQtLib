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

#ifndef ZSApps_TestDiagram_Units_Frequency_h
#define ZSApps_TestDiagram_Units_Frequency_h

#include "ZSPhysVal/ZSPhysTreeEntryGrpPhysUnits.h"
#include "ZSPhysVal/ZSPhysTreeEntryPhysUnit.h"
#include "ZSPhysVal/ZSPhysUnit.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace Diagram
{
//******************************************************************************
class CPhysSizeFrequency : public ZS::PhysVal::CUnitsTreeEntryGrpPhysUnits
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeFrequency(ZS::System::CIdxTreeEntry* i_pParentBranch);
    virtual ~CPhysSizeFrequency();
private: // instance members
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryHertz;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryKiloHertz;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryMegaHertz;
    ZS::PhysVal::CUnitsTreeEntryPhysUnit m_treeEntryGigaHertz;
public: // instance members
    ZS::PhysVal::CUnit Hertz;
    ZS::PhysVal::CUnit KiloHertz;
    ZS::PhysVal::CUnit MegaHertz;
    ZS::PhysVal::CUnit GigaHertz;
    ZS::PhysVal::CUnit Hz;
    ZS::PhysVal::CUnit kHz;
    ZS::PhysVal::CUnit MHz;
    ZS::PhysVal::CUnit GHz;

}; // class CPhysSizeFrequency

} // namespace Diagram

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestDiagram_Units_Frequency_h
