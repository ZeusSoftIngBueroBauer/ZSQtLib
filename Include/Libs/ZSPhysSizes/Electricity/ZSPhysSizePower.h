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

#ifndef ZSPhysSizes_Electricity_Power_h
#define ZSPhysSizes_Electricity_Power_h

#include <math.h>
#include "ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Electricity
{
//******************************************************************************
class ZSPHYSSIZESELECTRICITYDLL_API CPhysSizePower : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizePower();
    ~CPhysSizePower();
public: // instance methods
    //CPhysUnit* YoktoWatt();
    //CPhysUnit* ZeptoWatt();
    //CPhysUnit* AttoWatt();
    //CPhysUnit* FemtoWatt();
    CPhysUnit* PicoWatt() { return &m_physUnitPicoWatt; }
    CPhysUnit* NanoWatt() { return &m_physUnitNanoWatt; }
    CPhysUnit* MicroWatt() { return &m_physUnitMicroWatt; }
    CPhysUnit* MilliWatt() { return &m_physUnitMilliWatt; }
    CPhysUnit* Watt() { return &m_physUnitWatt; }
    CPhysUnit* KiloWatt() { return &m_physUnitKiloWatt; }
    CPhysUnit* MegaWatt() { return &m_physUnitMegaWatt; }
    CPhysUnit* GigaWatt() { return &m_physUnitGigaWatt; }
    //CPhysUnit* TeraWatt();
    //CPhysUnit* PetaWatt();
    //CPhysUnit* ExaWatt();
    //CPhysUnit* ZettaWatt();
    //CPhysUnit* YottaWatt();
    CPhysUnit* dBWatt() { return &m_physUnitdBWatt; }
    CPhysUnit* dBMilliWatt() { return &m_physUnitdBMilliWatt; }
    CPhysUnit* dBm() { return &m_physUnitdBMilliWatt; }
    CPhysUnit* dBMicroWatt() { return &m_physUnitdBMicroWatt; }
    CPhysUnit* dBNanoWatt() { return &m_physUnitdBNanoWatt; }
    CPhysUnit* dBPicoWatt() { return &m_physUnitdBPicoWatt; }
protected: // instance members
    //CPhysUnit m_physUnitYoktoWatt;
    //CPhysUnit m_physUnitZeptoWatt;
    //CPhysUnit m_physUnitAttoWatt;
    //CPhysUnit m_physUnitFemtoWatt;
    CPhysUnit m_physUnitPicoWatt;
    CPhysUnit m_physUnitNanoWatt;
    CPhysUnit m_physUnitMicroWatt;
    CPhysUnit m_physUnitMilliWatt;
    CPhysUnit m_physUnitWatt;
    CPhysUnit m_physUnitKiloWatt;
    CPhysUnit m_physUnitMegaWatt;
    CPhysUnit m_physUnitGigaWatt;
    //CPhysUnit m_physUnitTeraWatt;
    //CPhysUnit m_physUnitPetaWatt;
    //CPhysUnit m_physUnitExaWatt;
    //CPhysUnit m_physUnitZettaWatt;
    //CPhysUnit m_physUnitYottaWatt;
    CPhysUnit m_physUnitdBWatt;
    CPhysUnit m_physUnitdBMilliWatt;
    CPhysUnit m_physUnitdBMicroWatt;
    CPhysUnit m_physUnitdBNanoWatt;
    CPhysUnit m_physUnitdBPicoWatt;

}; // class CPhysSizePower

} // namespace Electricity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Electricity_Power_h
