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

#ifndef ZSPhysSizes_Electricity_Resistance_h
#define ZSPhysSizes_Electricity_Resistance_h

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
const QString c_strSymbolOhm(QChar(0x03A9));

const double c_fRefValResistance_50Ohm = 50.0;
const double c_fRefValResistance_10log50Ohm = 10.0*log10(50.0);


//******************************************************************************
class ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeResistance : public CPhysSize
//******************************************************************************
{
public: // ctors and dtor
    CPhysSizeResistance( CUnitsPool* i_pUnitsPool );
    ~CPhysSizeResistance();
public: // overridables of base class CPhysSize
    virtual double getRefVal( CPhysUnit* i_pPhysUnitRef ) const;
public: // instance methods
    //CPhysUnit* YoktoOhm();
    //CPhysUnit* ZeptoOhm();
    //CPhysUnit* AttoOhm();
    //CPhysUnit* FemtoOhm();
    //CPhysUnit* PicoOhm();
    CPhysUnit* NanoOhm() { return &m_physUnitNanoOhm; }
    CPhysUnit* MicroOhm() { return &m_physUnitMicroOhm; }
    CPhysUnit* MilliOhm() { return &m_physUnitMilliOhm; }
    CPhysUnit* Ohm() { return &m_physUnitOhm; }
    CPhysUnit* KiloOhm() { return &m_physUnitKiloOhm; }
    CPhysUnit* MegaOhm() { return &m_physUnitMegaOhm; }
    //CPhysUnit* GigaOhm();
    //CPhysUnit* TeraOhm();
    //CPhysUnit* PetaOhm();
    //CPhysUnit* ExaOhm();
    //CPhysUnit* ZettaOhm();
    //CPhysUnit* YottaOhm();
protected: // instance members
    //CPhysUnit m_physUnitYoktoOhm;
    //CPhysUnit m_physUnitZeptoOhm;
    //CPhysUnit m_physUnitAttoOhm;
    //CPhysUnit m_physUnitFemtoOhm;
    //CPhysUnit m_physUnitPicoOhm;
    CPhysUnit m_physUnitNanoOhm;
    CPhysUnit m_physUnitMicroOhm;
    CPhysUnit m_physUnitMilliOhm;
    CPhysUnit m_physUnitOhm;
    CPhysUnit m_physUnitKiloOhm;
    CPhysUnit m_physUnitMegaOhm;
    //CPhysUnit m_physUnitGigaOhm;
    //CPhysUnit m_physUnitTeraOhm;
    //CPhysUnit m_physUnitPetaOhm;
    //CPhysUnit m_physUnitExaOhm;
    //CPhysUnit m_physUnitZettaOhm;
    //CPhysUnit m_physUnitYottaOhm;

}; // class CPhysSizeResistance

} // namespace Electricity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Electricity_Resistance_h
