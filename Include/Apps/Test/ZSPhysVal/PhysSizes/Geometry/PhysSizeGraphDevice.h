/*******************************************************************************

Copyright 2004 - 2020 by ZeusSoft, Ing. Buero Bauer
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

#ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_GraphDevice_h
#define ZSApps_TestPhysVal_PhysSizes_Geometry_GraphDevice_h

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
class CPhysSizeGraphDevice : public ZS::PhysVal::CPhysSizeTreeEntry
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeGraphDevice(ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree);
    CPhysSizeGraphDevice(ZS::PhysVal::CPhysScienceFieldTreeEntry* i_pParentBranch);
    CPhysSizeGraphDevice(CPhysSizeGraphDevice&& i_other) = delete;
    CPhysSizeGraphDevice(CPhysSizeGraphDevice& i_other) = delete;
    CPhysSizeGraphDevice(const CPhysSizeGraphDevice& i_other) = delete;
    virtual ~CPhysSizeGraphDevice();
public: // operators
    CPhysSizeGraphDevice& operator=(CPhysSizeGraphDevice& i_other) = delete;
    CPhysSizeGraphDevice& operator=(const CPhysSizeGraphDevice& i_other) = delete;
    CPhysSizeGraphDevice& operator=(CPhysSizeGraphDevice&& i_other) = delete;
private: // instance members
    ZS::PhysVal::CPhysUnitTreeEntry m_treeEntryPixel;
public: // instance members
    ZS::PhysVal::CPhysUnit Pixel;
    ZS::PhysVal::CPhysUnit px;

}; // CPhysSizeGraphDevice

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_GraphDevice_h
