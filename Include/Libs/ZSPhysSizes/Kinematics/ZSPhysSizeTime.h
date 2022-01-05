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

#ifndef ZSPhysSizes_Kinematics_Time_h
#define ZSPhysSizes_Kinematics_Time_h

#include "ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Kinematics
{
//******************************************************************************
class ZSPHYSSIZESKINEMATICSDLL_API CPhysSizeTime : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeTime();
    ~CPhysSizeTime();
public: // instance methods
    CPhysUnit* NanoSeconds() { return &m_physUnitNanoSeconds; }
    CPhysUnit* MicroSeconds() { return &m_physUnitMicroSeconds; }
    CPhysUnit* MilliSeconds() { return &m_physUnitMilliSeconds; }
    CPhysUnit* Seconds() { return &m_physUnitSeconds; }
    CPhysUnit* Minutes() { return &m_physUnitMinutes; }
    CPhysUnit* Hours() { return &m_physUnitHours; }
    CPhysUnit* Days() { return &m_physUnitDays; }
protected: // instance members
    CPhysUnit m_physUnitNanoSeconds;
    CPhysUnit m_physUnitMicroSeconds;
    CPhysUnit m_physUnitMilliSeconds;
    CPhysUnit m_physUnitSeconds;
    CPhysUnit m_physUnitMinutes;
    CPhysUnit m_physUnitHours;
    CPhysUnit m_physUnitDays;

}; // class CPhysSizeTime

} // namespace Kinematics

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Kinematics_Time_h
