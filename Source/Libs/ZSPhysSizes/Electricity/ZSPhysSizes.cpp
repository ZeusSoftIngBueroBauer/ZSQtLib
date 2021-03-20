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

#include "ZSPhysSizes/Electricity/ZSPhysSizes.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Electricity;


/*******************************************************************************
namespace Electricity
*******************************************************************************/

static CPhysSizeCurrent*    s_pPhysSizeCurrent = nullptr;
static CPhysSizeVoltage*    s_pPhysSizeVoltage = nullptr;
static CPhysSizeResistance* s_pPhysSizeResistance = nullptr;
static CPhysSizePower*      s_pPhysSizePower = nullptr;

//------------------------------------------------------------------------------
void ZS::PhysVal::Electricity::createPhysSizes()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeCurrent == nullptr )
    {
        s_pPhysSizeCurrent = new CPhysSizeCurrent();
    }
    if( s_pPhysSizeVoltage == nullptr )
    {
        s_pPhysSizeVoltage = new CPhysSizeVoltage();
    }
    if( s_pPhysSizeResistance == nullptr )
    {
        s_pPhysSizeResistance = new CPhysSizeResistance();
    }
    if( s_pPhysSizePower == nullptr )
    {
        s_pPhysSizePower = new CPhysSizePower();
    }

    // Add conversion routines to convert units between different physical sizes
    //==========================================================================

    Voltage()->addFctConvert( // P = U²/R
        /* pPhysSizeDst   */ Power(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_SQRxDIVr );

    Voltage()->addFctConvert( // I = U/R
        /* pPhysSizeDst   */ Current(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_xDIVr );

    Power()->addFctConvert( // U = sqrt(P*R)
        /* pPhysSizeDst   */ Voltage(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_SQRT_xMULr_ );

    Power()->addFctConvert( // I = sqrt(P/R)
        /* pPhysSizeDst   */ Current(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_SQRT_xDIVr_ );

    Current()->addFctConvert( // P = I²*R
        /* pPhysSizeDst   */ Power(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_SQRxDIVr );

    Current()->addFctConvert( // U = I*R
        /* pPhysSizeDst   */ Voltage(),
        /* pPhysSizeRef   */ Resistance(),
        /* fctConvert     */ EFctConvert_xMULr );

} // createPhysSizes

//------------------------------------------------------------------------------
void ZS::PhysVal::Electricity::deletePhysSizes()
//------------------------------------------------------------------------------
{
    delete s_pPhysSizeCurrent;
    s_pPhysSizeCurrent = nullptr;

    delete s_pPhysSizeVoltage;
    s_pPhysSizeVoltage = nullptr;

    delete s_pPhysSizeResistance;
    s_pPhysSizeResistance = nullptr;

    delete s_pPhysSizePower;
    s_pPhysSizePower = nullptr;

} // deletePhysSizes

//------------------------------------------------------------------------------
CPhysSizeCurrent* ZS::PhysVal::Electricity::Current()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeCurrent == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeCurrent;
}

//------------------------------------------------------------------------------
CPhysSizeVoltage* ZS::PhysVal::Electricity::Voltage()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeVoltage == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeVoltage;
}

//------------------------------------------------------------------------------
CPhysSizeResistance* ZS::PhysVal::Electricity::Resistance()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeResistance == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeResistance;
}

//------------------------------------------------------------------------------
CPhysSizePower* ZS::PhysVal::Electricity::Power()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizePower == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizePower;
}
