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

#include "ZSPhysSizes/Geometry/ZSPhysSizes.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::PhysVal::Geometry;


/*******************************************************************************
namespace Geometry
*******************************************************************************/

static CPhysSizeAngle*       s_pPhysSizeAngle       = nullptr;
static CPhysSizeGraphDevice* s_pPhysSizeGraphDevice = nullptr;
static CPhysSizeLength*      s_pPhysSizeLength      = nullptr;

//------------------------------------------------------------------------------
void ZS::PhysVal::Geometry::createPhysSizes()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeAngle == nullptr )
    {
        s_pPhysSizeAngle = new CPhysSizeAngle();
    }
    if( s_pPhysSizeGraphDevice == nullptr )
    {
        s_pPhysSizeGraphDevice = new CPhysSizeGraphDevice();
    }
    if( s_pPhysSizeLength == nullptr )
    {
        s_pPhysSizeLength = new CPhysSizeLength();
    }

} // createPhysSizes

//------------------------------------------------------------------------------
void ZS::PhysVal::Geometry::deletePhysSizes()
//------------------------------------------------------------------------------
{
    delete s_pPhysSizeLength;
    s_pPhysSizeLength = nullptr;

    delete s_pPhysSizeGraphDevice;
    s_pPhysSizeGraphDevice = nullptr;

    delete s_pPhysSizeAngle;
    s_pPhysSizeAngle = nullptr;

} // deletePhysSizes

//------------------------------------------------------------------------------
CPhysSizeAngle* ZS::PhysVal::Geometry::Angle()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeAngle == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeAngle;
}

//------------------------------------------------------------------------------
CPhysSizeGraphDevice* ZS::PhysVal::Geometry::GraphDevice()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeGraphDevice == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeGraphDevice;
}

//------------------------------------------------------------------------------
CPhysSizeLength* ZS::PhysVal::Geometry::Length()
//------------------------------------------------------------------------------
{
    if( s_pPhysSizeLength == nullptr )
    {
        throw ZS::System::CException( __FILE__, __LINE__, EResultSingletonClassNotInstantiated );
    }
    return s_pPhysSizeLength;
}
