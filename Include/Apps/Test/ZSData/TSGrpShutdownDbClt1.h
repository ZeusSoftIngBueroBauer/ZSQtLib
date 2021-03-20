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

#ifndef ZSApps_TestData_TSGrpShutdownDbClt1_h
#define ZSApps_TestData_TSGrpShutdownDbClt1_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSData/ZSDataDllMain.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace Data
{
class CTest;

//******************************************************************************
class CTSGrpShutdownDbClt1 : public ZS::Test::CTestStepGroup
//******************************************************************************
{
public: // ctors and dtor
    CTSGrpShutdownDbClt1( CTest* i_pTest, ZS::Test::CTestStepGroup* i_pTSGrpParent = nullptr );
    ~CTSGrpShutdownDbClt1();

}; // class CTSGrpShutdownDbClt1

} // namespace Data

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestData_TSGrpShutdownDbClt1_h
