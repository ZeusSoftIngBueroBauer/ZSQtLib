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

#ifndef ZSPhysSizes_Geometry_Length_h
#define ZSPhysSizes_Geometry_Length_h

#include "ZSPhysSizes/Geometry/ZSPhysSizesGeometryDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Geometry
{
//******************************************************************************
class ZSPHYSSIZESGEOMETRYDLL_API CPhysSizeLength : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeLength();
    ~CPhysSizeLength();
public: // instance methods
    //CPhysUnit* YoktoMeter();
    //CPhysUnit* ZeptoMeter();
    //CPhysUnit* AttoMeter();
    //CPhysUnit* FemtoMeter();
    CPhysUnit* PicoMeter() { return &m_physUnitPicoMeter; }
    CPhysUnit* NanoMeter() { return &m_physUnitNanoMeter; }
    CPhysUnit* MicroMeter() { return &m_physUnitMicroMeter; }
    CPhysUnit* MilliMeter() { return &m_physUnitMilliMeter; }
    CPhysUnit* CentiMeter() { return &m_physUnitCentiMeter; }
    CPhysUnit* DeziMeter() { return &m_physUnitDeziMeter; }
    CPhysUnit* Meter() { return &m_physUnitMeter; }
    CPhysUnit* KiloMeter() { return &m_physUnitKiloMeter; }
    //CPhysUnit* MegaMeter();
    //CPhysUnit* GigaMeter();
    //CPhysUnit* TeraMeter();
    //CPhysUnit* PetaMeter();
    //CPhysUnit* ExaMeter();
    //CPhysUnit* ZettaMeter();
    //CPhysUnit* YottaMeter();
    CPhysUnit* Inch() { return &m_physUnitInch; }
    CPhysUnit* Foot() { return &m_physUnitFoot; }
    CPhysUnit* Yard() { return &m_physUnitYard; }
    CPhysUnit* Mile() { return &m_physUnitMile; }
    CPhysUnit* NauticalMile() { return &m_physUnitNauticalMile; }
protected: // instance members
    //CPhysUnit m_physUnitYoktoMeter;
    //CPhysUnit m_physUnitZeptoMeter;
    //CPhysUnit m_physUnitAttoMeter;
    //CPhysUnit m_physUnitFemtoMeter;
    CPhysUnit m_physUnitPicoMeter;
    CPhysUnit m_physUnitNanoMeter;
    CPhysUnit m_physUnitMicroMeter;
    CPhysUnit m_physUnitMilliMeter;
    CPhysUnit m_physUnitCentiMeter;
    CPhysUnit m_physUnitDeziMeter;
    CPhysUnit m_physUnitMeter;
    CPhysUnit m_physUnitKiloMeter;
    //CPhysUnit m_physUnitMegaMeter;
    //CPhysUnit m_physUnitGigaMeter;
    //CPhysUnit m_physUnitTeraMeter;
    //CPhysUnit m_physUnitPetaMeter;
    //CPhysUnit m_physUnitExaMeter;
    //CPhysUnit m_physUnitZettaMeter;
    //CPhysUnit m_physUnitYottaMeter;
    CPhysUnit m_physUnitInch;
    CPhysUnit m_physUnitFoot;
    CPhysUnit m_physUnitYard;
    CPhysUnit m_physUnitMile;
    CPhysUnit m_physUnitNauticalMile;

}; // CPhysSizeLength

} // namespace Geometry

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Geometry_Length_h
