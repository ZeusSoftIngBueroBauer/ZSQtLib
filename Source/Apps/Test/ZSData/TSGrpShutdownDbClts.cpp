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

#include <QtCore/qtimer.h>

#include "TSGrpShutdownDbClts.h"
#include "TSGrpShutdownDbClt1.h"
#include "TSGrpShutdownDbClt2.h"
#include "TSGrpShutdownDbClt3.h"
#include "Test.h"

#include "ZSData/ZSDatabaseClient.h"
#include "ZSData/ZSDatabase.h"
#include "ZSData/ZSDataSet.h"
#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepAdminObjPool.h"
#include "ZSSys/ZSSysException.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"

using namespace ZS::System;
using namespace ZS::Data;
using namespace ZS::Ipc;
using namespace ZS::PhysVal;
using namespace ZS::Apps::Test::Data;


/*******************************************************************************
class CTSGrpShutdownDbClts : public ZS::Test::CTestStepGroup
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTSGrpShutdownDbClts::CTSGrpShutdownDbClts( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent ) :
//------------------------------------------------------------------------------
    ZS::Test::CTestStepGroup(i_pTest, "DbClts", i_pTSGrpParent)
{
    new CTSGrpShutdownDbClt3(i_pTest, this);
    new CTSGrpShutdownDbClt2(i_pTest, this);
    new CTSGrpShutdownDbClt1(i_pTest, this);

} // ctor

//------------------------------------------------------------------------------
CTSGrpShutdownDbClts::~CTSGrpShutdownDbClts()
//------------------------------------------------------------------------------
{
} // dtor
