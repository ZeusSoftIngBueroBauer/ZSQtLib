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

#ifndef ZSApps_TestSysEnums_Test_h
#define ZSApps_TestSysEnums_Test_h

#include "ZSTest/ZSTest.h"

namespace ZS
{
namespace Apps
{
namespace Test
{
namespace SysEnums
{
//******************************************************************************
class CTest : public ZS::Test::CTest
//******************************************************************************
{
    Q_OBJECT
public: // ctors and dtor
    CTest();
    ~CTest();
public slots: // test step methods
    void doTestStepEnumEntryClassMethodsEnumerator2Str( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsEnumerator2Val( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsStr2Enumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsStr2Val( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsVal2Enumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsVal2Str( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsIsValidEnumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsInitStr2EnumeratorMaps( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsMapStr2Enumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumEntryClassMethodsMapStr2Val( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods
    void doTestStepEnumClassTemplateZSSysModeClassMethodToString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeClassMethodFromString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeCtors( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorAssign( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeOperatorIncDec( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeInstMethodToString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateZSSysModeForLoop( ZS::Test::CTestStep* i_pTestStep );
public slots: // test step methods
    void doTestStepEnumClassTemplateUserDefinedClassMethodToString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedClassMethodToEnumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedClassMethodToValue( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedClassMethodFromString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedClassMethodFromValue( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedCtors( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorAssign( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumerator( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorCompareWithInt( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorCompareWithCharPtr( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorCompareWithQString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorCompareWithEnumTemplate( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedOperatorIncDec( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedInstMethodToString( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedInstMethodToValue( ZS::Test::CTestStep* i_pTestStep );
    void doTestStepEnumClassTemplateUserDefinedForLoop( ZS::Test::CTestStep* i_pTestStep );

}; // class CTest

} // namespace SysEnums

} // namespace Test

} // namespace Apps

} // namespace ZS

#endif // #ifndef ZSApps_TestSysEnums_Test_h
