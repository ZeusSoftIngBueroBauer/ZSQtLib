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

#ifndef ZSDraw_PhysSizesGeometry_h
#define ZSDraw_PhysSizesGeometry_h

#include "ZSDraw/ZSDrawDllMain.h"

#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace Draw
{
//******************************************************************************
class ZSDRAWDLL_API CPhysSizeGeometry : public ZS::PhysVal::CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeGeometry( double i_fDpmm = 3.78 );
    ~CPhysSizeGeometry();
public: // instance methods
    ZS::PhysVal::CPhysUnit* Pixel() { return &m_physUnitPixel; }
    ZS::PhysVal::CPhysUnit* MicroMeter() { return &m_physUnitMicroMeter; }
    ZS::PhysVal::CPhysUnit* MilliMeter() { return &m_physUnitMilliMeter; }
    ZS::PhysVal::CPhysUnit* CentiMeter() { return &m_physUnitCentiMeter; }
    ZS::PhysVal::CPhysUnit* DeziMeter() { return &m_physUnitDeziMeter; }
    ZS::PhysVal::CPhysUnit* Meter() { return &m_physUnitMeter; }
    ZS::PhysVal::CPhysUnit* KiloMeter() { return &m_physUnitKiloMeter; }
public: // instance methods
    void setDpmm( double i_fDpmm );
    double getDpmm() const;
protected: // instance members
    ZS::PhysVal::CPhysUnit m_physUnitPixel;
    ZS::PhysVal::CPhysUnit m_physUnitMicroMeter;
    ZS::PhysVal::CPhysUnit m_physUnitMilliMeter;
    ZS::PhysVal::CPhysUnit m_physUnitCentiMeter;
    ZS::PhysVal::CPhysUnit m_physUnitDeziMeter;
    ZS::PhysVal::CPhysUnit m_physUnitMeter;
    ZS::PhysVal::CPhysUnit m_physUnitKiloMeter;

}; // CPhysSizeGeometry

} // namespace Draw

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Geometry_Length_h
