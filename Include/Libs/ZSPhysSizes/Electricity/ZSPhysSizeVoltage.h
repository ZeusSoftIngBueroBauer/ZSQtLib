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

#ifndef ZSPhysSizes_Electricity_Voltage_h
#define ZSPhysSizes_Electricity_Voltage_h

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
const double c_fRefValdBu_0775V = sqrt(0.6);

//******************************************************************************
class ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeVoltage : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeVoltage();
    ~CPhysSizeVoltage();
public: // instance methods
    //CPhysUnit* YoktoVolt();
    //CPhysUnit* ZeptoVolt();
    //CPhysUnit* AttoVolt();
    //CPhysUnit* FemtoVolt();
    CPhysUnit* PicoVolt() { return &m_physUnitPicoVolt; }
    CPhysUnit* NanoVolt() { return &m_physUnitNanoVolt; }
    CPhysUnit* MicroVolt() { return &m_physUnitMicroVolt; }
    CPhysUnit* MilliVolt() { return &m_physUnitMilliVolt; }
    CPhysUnit* Volt() { return &m_physUnitVolt; }
    CPhysUnit* KiloVolt() { return &m_physUnitKiloVolt; }
    CPhysUnit* MegaVolt() { return &m_physUnitMegaVolt; }
    //CPhysUnit* GigaVolt();
    //CPhysUnit* TeraVolt();
    //CPhysUnit* PetaVolt();
    //CPhysUnit* ExaVolt();
    //CPhysUnit* ZettaVolt();
    //CPhysUnit* YottaVolt();
    CPhysUnit* dBVolt() { return &m_physUnitdBVolt; }
    CPhysUnit* dBu() { return &m_physUnitdBu; }
    CPhysUnit* dBMilliVolt() { return &m_physUnitdBMilliVolt; }
    CPhysUnit* dBMicroVolt() { return &m_physUnitdBMicroVolt; }
    CPhysUnit* dBNanoVolt() { return &m_physUnitdBNanoVolt; }
    CPhysUnit* dBPicoVolt() { return &m_physUnitdBPicoVolt; }
protected: // instance members
    //CPhysUnit m_physUnitYoktoVolt;
    //CPhysUnit m_physUnitZeptoVolt;
    //CPhysUnit m_physUnitAttoVolt;
    //CPhysUnit m_physUnitFemtoVolt;
    CPhysUnit m_physUnitPicoVolt;
    CPhysUnit m_physUnitNanoVolt;
    CPhysUnit m_physUnitMicroVolt;
    CPhysUnit m_physUnitMilliVolt;
    CPhysUnit m_physUnitVolt;
    CPhysUnit m_physUnitKiloVolt;
    CPhysUnit m_physUnitMegaVolt;
    //CPhysUnit m_physUnitGigaVolt;
    //CPhysUnit m_physUnitTeraVolt;
    //CPhysUnit m_physUnitPetaVolt;
    //CPhysUnit m_physUnitExaVolt;
    //CPhysUnit m_physUnitZettaVolt;
    //CPhysUnit m_physUnitYottaVolt;
    CPhysUnit m_physUnitdBVolt;
    CPhysUnit m_physUnitdBu;
    CPhysUnit m_physUnitdBMilliVolt;
    CPhysUnit m_physUnitdBMicroVolt;
    CPhysUnit m_physUnitdBNanoVolt;
    CPhysUnit m_physUnitdBPicoVolt;

}; // class CPhysSizeVoltage

} // namespace Electricity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Electricity_Voltage_h
