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

#ifndef ZSPhysSizes_Kinematics_Frequency_h
#define ZSPhysSizes_Kinematics_Frequency_h

#include "ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.h"
#include "ZSPhysVal/ZSPhysSize.h"
#include "ZSPhysVal/ZSPhysUnits.h"

namespace ZS
{
namespace PhysVal
{
namespace Kinematics
{
const double c_fRefValSampleRate_1000Hz = 1000.0;

//******************************************************************************
class ZSPHYSSIZESKINEMATICSDLL_API CPhysSizeFrequency : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeFrequency( CUnitsPool* i_pUnitsPool );
    ~CPhysSizeFrequency();
public: // instance methods
    //CPhysUnit* YoktoHertz();
    //CPhysUnit* ZeptoHertz();
    //CPhysUnit* AttoHertz();
    //CPhysUnit* FemtoHertz();
    //CPhysUnit* PicoHertz();
    //CPhysUnit* NanoHertz();
    //CPhysUnit* MicroHertz();
    //CPhysUnit* MilliHertz();
    CPhysUnit* Hertz() { return &m_physUnitHertz; }
    CPhysUnit* KiloHertz() { return &m_physUnitKiloHertz; }
    CPhysUnit* MegaHertz() { return &m_physUnitMegaHertz; }
    CPhysUnit* GigaHertz() { return &m_physUnitGigaHertz; }
    //CPhysUnit* TeraHertz();
    //CPhysUnit* PetaHertz();
    //CPhysUnit* ExaHertz();
    //CPhysUnit* ZettaHertz();
    //CPhysUnit* YottaHertz();
protected: // instance members
    //CPhysUnit m_physUnitYoktoHertz;
    //CPhysUnit m_physUnitZeptoHertz;
    //CPhysUnit m_physUnitAttoHertz;
    //CPhysUnit m_physUnitFemtoHertz;
    //CPhysUnit m_physUnitPicoHertz;
    //CPhysUnit m_physUnitNanoHertz;
    //CPhysUnit m_physUnitMicroHertz;
    //CPhysUnit m_physUnitMilliHertz;
    CPhysUnit m_physUnitHertz;
    CPhysUnit m_physUnitKiloHertz;
    CPhysUnit m_physUnitMegaHertz;
    CPhysUnit m_physUnitGigaHertz;
    //CPhysUnit m_physUnitTeraHertz;
    //CPhysUnit m_physUnitPetaHertz;
    //CPhysUnit m_physUnitExaHertz;
    //CPhysUnit m_physUnitZettaHertz;
    //CPhysUnit m_physUnitYottaHertz;

}; // class CPhysSizeFrequency

} // namespace Kinematics

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Kinematics_Frequency_h
