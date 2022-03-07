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

#ifndef ZSApps_TestTemplate_Test_h
#define ZSApps_TestTemplate_Test_h

#include "ZSTest/ZSTest.h"
#include "ZSTest/ZSTestStepAdminObj.h"

class QTimer;

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace Template
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // type definitions and constants
public: // ctors and dtor
    CTest();
    ~CTest();
public slots: // test step methods (GrpTemplate)
    void doTestStepGrpTemplateTestStep1( ZS::Test::CTestStep* i_pTestStep );
protected: // instance members

}; // class CTest

} // namespace Template

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestTemplate_Test_h
