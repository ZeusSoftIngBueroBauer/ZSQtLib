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

#ifndef ZSPhysSizes_Electricity_PhysSizes_h
#define ZSPhysSizes_Electricity_PhysSizes_h

#include "ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.h"
#include "ZSPhysSizes/Electricity/ZSPhysSizeCurrent.h"
#include "ZSPhysSizes/Electricity/ZSPhysSizePower.h"
#include "ZSPhysSizes/Electricity/ZSPhysSizeResistance.h"
#include "ZSPhysSizes/Electricity/ZSPhysSizeVoltage.h"


/*******************************************************************************
interface description of exported methods
*******************************************************************************/

namespace ZS
{
namespace PhysVal
{
namespace Electricity
{
class CPhysSizeCurrent;
class CPhysSizeVoltage;
class CPhysSizeResistance;
class CPhysSizePower;

ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeCurrent* Current();
ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeVoltage* Voltage();
ZSPHYSSIZESELECTRICITYDLL_API CPhysSizeResistance* Resistance();
ZSPHYSSIZESELECTRICITYDLL_API CPhysSizePower* Power();

} // namespace Electricity

} // namespace PhysVal

} // namespace ZS

#endif // #ifndef ZSPhysSizes_Electricity_PhysSizes_h
