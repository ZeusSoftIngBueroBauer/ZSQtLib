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

#ifndef ZSPhysSizes_Electricity_Current_h
#define ZSPhysSizes_Electricity_Current_h

#include "ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Electricity
{
const double c_fRefValCurrent_1A = 1.0;


//******************************************************************************
class ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeCurrent : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeCurrent( CUnitsPool* i_pUnitsPool );
    ~CPhysSizeCurrent();
public: // instance methods
    //CPhysUnit* YoktoAmpere();
    //CPhysUnit* ZeptoAmpere();
    //CPhysUnit* AttoAmpere();
    //CPhysUnit* FemtoAmpere();
    CPhysUnit* PicoAmpere() { return &m_physUnitPicoAmpere; }
    CPhysUnit* NanoAmpere() { return &m_physUnitNanoAmpere; }
    CPhysUnit* MicroAmpere() { return &m_physUnitMicroAmpere; }
    CPhysUnit* MilliAmpere() { return &m_physUnitMilliAmpere; }
    CPhysUnit* Ampere() { return &m_physUnitAmpere; }
    //CPhysUnit* KiloAmpere();
    //CPhysUnit* MegaAmpere();
    //CPhysUnit* GigaAmpere();
    //CPhysUnit* TeraAmpere();
    //CPhysUnit* PetaAmpere();
    //CPhysUnit* ExaAmpere();
    //CPhysUnit* ZettaAmpere();
    //CPhysUnit* YottaAmpere();
protected: // instance members
    //CPhysUnit m_physUnitYoktoAmpere;
    //CPhysUnit m_physUnitZeptoAmpere;
    //CPhysUnit m_physUnitAttoAmpere;
    //CPhysUnit m_physUnitFemtoAmpere;
    CPhysUnit m_physUnitPicoAmpere;
    CPhysUnit m_physUnitNanoAmpere;
    CPhysUnit m_physUnitMicroAmpere;
    CPhysUnit m_physUnitMilliAmpere;
    CPhysUnit m_physUnitAmpere;
    //CPhysUnit m_physUnitKiloAmpere;
    //CPhysUnit m_physUnitMegaAmpere;
    //CPhysUnit m_physUnitGigaAmpere;
    //CPhysUnit m_physUnitTeraAmpere;
    //CPhysUnit m_physUnitPetaAmpere;
    //CPhysUnit m_physUnitExaAmpere;
    //CPhysUnit m_physUnitZettaAmpere;
    //CPhysUnit m_physUnitYottaAmpere;

}; // class CPhysSizeCurrent

} // namespace Electricity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Electricity_Current_h
