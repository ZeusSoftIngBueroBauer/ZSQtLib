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

#include "ZSPhysSizes/Kinematics/ZSPhysSizes.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeBitRate.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeFrequency.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeSamples.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeTime.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizeVelocity.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal;
using namespace ZS::PhysVal::Kinematics;


/*******************************************************************************
namespace Kinematics
*******************************************************************************/

static CPhysSizeTime*      s_pPhysSizeTime = nullptr;
static CPhysSizeFrequency* s_pPhysSizeFrequency = nullptr;
static CPhysSizeVelocity*  s_pPhysSizeVelocity = nullptr;
static CPhysSizeBitRate*   s_pPhysSizeBitRate = nullptr;
static CPhysSizeSamples*   s_pPhysSizeSamples = nullptr;


//------------------------------------------------------------------------------
void ZS::PhysVal::Kinematics::createPhysSizes()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeTime == nullptr )
    {
        s_pPhysSizeTime = new CPhysSizeTime();
    }
    if( s_pPhysSizeFrequency == nullptr )
    {
        s_pPhysSizeFrequency = new CPhysSizeFrequency();
    }
    if( s_pPhysSizeVelocity == nullptr )
    {
        s_pPhysSizeVelocity = new CPhysSizeVelocity();
    }
    if( s_pPhysSizeBitRate == nullptr )
    {
        s_pPhysSizeBitRate = new CPhysSizeBitRate();
    }
    if( s_pPhysSizeSamples == nullptr )
    {
        s_pPhysSizeSamples = new CPhysSizeSamples();
    }

} // createPhysSizes

//------------------------------------------------------------------------------
void ZS::PhysVal::Kinematics::deletePhysSizes()
//------------------------------------------------------------------------------
{
    delete s_pPhysSizeTime;
    s_pPhysSizeTime = nullptr;

    delete s_pPhysSizeFrequency;
    s_pPhysSizeFrequency = nullptr;

    delete s_pPhysSizeVelocity;
    s_pPhysSizeVelocity = nullptr;

    delete s_pPhysSizeBitRate;
    s_pPhysSizeBitRate = nullptr;

    delete s_pPhysSizeSamples;
    s_pPhysSizeSamples = nullptr;

} // deletePhysSizes

//------------------------------------------------------------------------------
CPhysSizeTime* ZS::PhysVal::Kinematics::Time()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeTime == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeTime;
}

//------------------------------------------------------------------------------
CPhysSizeFrequency* ZS::PhysVal::Kinematics::Frequency()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeFrequency == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeFrequency;
}

//------------------------------------------------------------------------------
CPhysSizeVelocity* ZS::PhysVal::Kinematics::Velocity()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeVelocity == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeVelocity;
}

//------------------------------------------------------------------------------
CPhysSizeBitRate* ZS::PhysVal::Kinematics::BitRate()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeBitRate == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeBitRate;
}

//------------------------------------------------------------------------------
CPhysSizeSamples* ZS::PhysVal::Kinematics::Samples()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeSamples == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeSamples;
}
