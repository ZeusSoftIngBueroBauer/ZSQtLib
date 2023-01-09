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

#ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_Angle_h
#define ZSApps_TestPhysVal_PhysSizes_Geometry_Angle_h

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
class CPhysSizeAngle : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeAngle( ZS::PhysVal::CIdxTreePhysSizes* i_pIdxTree );
    CPhysSizeAngle(ZS::System::CIdxTreeEntry* i_pParentBranch);
    CPhysSizeAngle(CPhysSizeAngle&& i_other) = delete;
    CPhysSizeAngle(CPhysSizeAngle& i_other) = delete;
    CPhysSizeAngle(const CPhysSizeAngle& i_other) = delete;
    virtual ~CPhysSizeAngle();
public: // operators
    CPhysSizeAngle& operator=(CPhysSizeAngle& i_other) = delete;
    CPhysSizeAngle& operator=(const CPhysSizeAngle& i_other) = delete;
    CPhysSizeAngle& operator=(CPhysSizeAngle&& i_other) = delete;
public: // class members
    static const QString c_strSymbolPhi;
public: // instance members
    ZS::PhysVal::CPhysUnit Rad;
    ZS::PhysVal::CPhysUnit Degree;

}; // CPhysSizeAngle

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_PhysSizes_Geometry_Angle_h
