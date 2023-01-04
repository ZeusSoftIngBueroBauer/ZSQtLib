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

#include "PhysSizes/Electricity/Electricity.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"

#if 0

using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::PhysVal::Electricity;


/*******************************************************************************
namespace ZS::Apps::Test::PhysVal::Electricity;
*******************************************************************************/

//------------------------------------------------------------------------------
const ZS::Apps::Test::PhysVal::Electricity::CPhysSizeCurrent& Current()
//------------------------------------------------------------------------------
{
    return CPhysScienceFieldElectricity::Current();
}

//------------------------------------------------------------------------------
const ZS::Apps::Test::PhysVal::Electricity::CPhysSizePower& Power()
//------------------------------------------------------------------------------
{
    return CPhysScienceFieldElectricity::Power();
}

//------------------------------------------------------------------------------
const ZS::Apps::Test::PhysVal::Electricity::CPhysSizeResistance& Resistance()
//------------------------------------------------------------------------------
{
    return CPhysScienceFieldElectricity::Resistance();
}

//------------------------------------------------------------------------------
const ZS::Apps::Test::PhysVal::Electricity::CPhysSizeVoltage& Voltage()
//------------------------------------------------------------------------------
{
    return CPhysScienceFieldElectricity::Voltage();
}


/*******************************************************************************
class CPhysScienceFieldElectricity
*******************************************************************************/

/*==============================================================================
private: // class members
==============================================================================*/

CPhysScienceFieldElectricity* CPhysScienceFieldElectricity::s_pTheInst = nullptr;

CPhysSizeCurrent*    CPhysScienceFieldElectricity::s_pPhysSizeCurrent = nullptr;
CPhysSizePower*      CPhysScienceFieldElectricity::s_pPhysSizePower = nullptr;
CPhysSizeResistance* CPhysScienceFieldElectricity::s_pPhysSizeResistance = nullptr;
CPhysSizeVoltage*    CPhysScienceFieldElectricity::s_pPhysSizeVoltage = nullptr;

/*==============================================================================
public: // class methods
==============================================================================*/

//------------------------------------------------------------------------------
const CPhysSizeCurrent& CPhysScienceFieldElectricity::Current()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated);
    }
    return *s_pPhysSizeCurrent;
}

//------------------------------------------------------------------------------
const CPhysSizePower& CPhysScienceFieldElectricity::Power()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated);
    }
    return *s_pPhysSizePower;
}

//------------------------------------------------------------------------------
const CPhysSizeResistance& CPhysScienceFieldElectricity::Resistance()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated);
    }
    return *s_pPhysSizeResistance;
}

//------------------------------------------------------------------------------
const CPhysSizeVoltage& CPhysScienceFieldElectricity::Voltage()
//------------------------------------------------------------------------------
{
    if( s_pTheInst == nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassNotInstantiated);
    }
    return *s_pPhysSizeVoltage;
}

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CPhysScienceFieldElectricity::CPhysScienceFieldElectricity(CIdxTreePhysSizes* i_pIdxTree)
//------------------------------------------------------------------------------
{
    if( s_pTheInst != nullptr) {
        throw CException(__FILE__, __LINE__, EResultSingletonClassAlreadyInstantiated);
    }

    s_pPhysSizeCurrent = new CPhysSizeCurrent(i_pIdxTree);
    s_pPhysSizePower = new CPhysSizePower(i_pIdxTree);
    s_pPhysSizeResistance = new CPhysSizeResistance(i_pIdxTree);
    s_pPhysSizeVoltage = new CPhysSizeVoltage(i_pIdxTree);

    // Add conversion routines to convert units between different physical sizes
    //==========================================================================

    s_pPhysSizeCurrent->addFctConvert( // P = I²*R
        /* pPhysSizeDst   */ s_pPhysSizePower,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_SQRxDIVr );

    s_pPhysSizeCurrent->addFctConvert( // U = I*R
        /* pPhysSizeDst   */ s_pPhysSizeVoltage,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_xMULr );

    s_pPhysSizePower->addFctConvert( // U = sqrt(P*R)
        /* pPhysSizeDst   */ s_pPhysSizeVoltage,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_SQRT_xMULr_ );

    s_pPhysSizePower->addFctConvert( // I = sqrt(P/R)
        /* pPhysSizeDst   */ s_pPhysSizeCurrent,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_SQRT_xDIVr_ );

    s_pPhysSizeVoltage->addFctConvert( // P = U²/R
        /* pPhysSizeDst   */ s_pPhysSizePower,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_SQRxDIVr );

    s_pPhysSizeVoltage->addFctConvert( // I = U/R
        /* pPhysSizeDst   */ s_pPhysSizeCurrent,
        /* pPhysSizeRef   */ s_pPhysSizeResistance,
        /* fctConvert     */ EFctConvert_xDIVr );

} // ctor

//------------------------------------------------------------------------------
CPhysScienceFieldElectricity::~CPhysScienceFieldElectricity()
//------------------------------------------------------------------------------
{
    delete s_pPhysSizeCurrent;
    s_pPhysSizeCurrent = nullptr;

    delete s_pPhysSizePower;
    s_pPhysSizePower = nullptr;

    delete s_pPhysSizeResistance;
    s_pPhysSizeResistance = nullptr;

    delete s_pPhysSizeVoltage;
    s_pPhysSizeVoltage = nullptr;

    s_pTheInst = nullptr;

} // dtor

#endif
