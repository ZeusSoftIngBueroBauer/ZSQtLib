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

#ifndef ZSApps_TestPhysVal_Units_h
#define ZSApps_TestPhysVal_Units_h

#include "Units/Electricity/Electricity.h"
#include "Units/Geometry/Geometry.h"
#include "Units/Kinematics/Kinematics.h"
#include "Units/Ratios/Ratios.h"
#include "Units/Quantities/Quantities.h"
#include "ZSPhysVal/ZSPhysUnitsIdxTree.h"
#include "ZSSys/ZSSysIdxTreeEntry.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace PhysVal
{
//******************************************************************************
class CIdxTreeUnitsPhysValTest : public ZS::PhysVal::CIdxTreeUnits
//******************************************************************************
{
public: // ctors and dtor
    CIdxTreeUnitsPhysValTest();
    virtual ~CIdxTreeUnitsPhysValTest();
public: // instance methods
    ZS::System::CIdxTreeEntry* scienceFieldsTreeEntry();
private: // instance members
    ZS::System::CIdxTreeEntry m_treeEntryScienceFields;
};

extern ZS::Apps::Test::PhysVal::CIdxTreeUnitsPhysValTest IdxTreeUnits;

extern CUnitsQuantities Quantities;
extern CUnitsRatios Ratio;
extern CPhysScienceFieldElectricity Electricity;
extern CPhysScienceFieldGeometry Geometry;
extern CPhysScienceFieldKinematics Kinematics;

} // namespace PhysVal

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestPhysVal_Units_h
