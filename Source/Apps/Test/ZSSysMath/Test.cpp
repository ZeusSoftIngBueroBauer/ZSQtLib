/*******************************************************************************

Copyright 2004 - 2023 by ZeusSoft, Ing. Buero Bauer
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

#include <QtCore/qfileinfo.h>
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QtCore/qrandom.h>
#endif
#include <QtCore/qthread.h>

#include "Test.h"

#include "ZSTest/ZSTestStep.h"
#include "ZSTest/ZSTestStepGroup.h"
#include "ZSSys/ZSSysErrLog.h"
#include "ZSSys/ZSSysMath.h"
#include "ZSSys/ZSSysMathScaleDivLines.h"
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::Math;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest(NameSpace(), "theInst")
{
    ZS::Test::CTestStep* pTestStep;

    // Angles
    //-------

    ZS::Test::CTestStepGroup* pGrpAngles = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Angles",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Rad2Deg",
        /* strOperation    */ "rad2Degree",
        /* pGrpParent      */ pGrpAngles,
        /* szDoTestStepFct */ SLOT(doTestStepConvertAngles(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", false}, {"Angle_deg", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /*  4 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 45.0}});
    /*  5 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /*  6 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -45.0}});
    /*  7 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /*  8 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", 90.0}});
    /*  9 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 10 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", -90.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f135Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 12 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f135Degrees_rad}, {"Normalize", false}, {"Angle_deg", 135.0}});
    /* 13 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f135Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 14 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f135Degrees_rad}, {"Normalize", false}, {"Angle_deg", -135.0}});
    /* 15 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_fPI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 16 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_fPI}, {"Normalize", false}, {"Angle_deg", 180.0}});
    /* 17 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_fPI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 18 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_fPI}, {"Normalize", false}, {"Angle_deg", -180.0}});
    /* 19 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f225Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 20 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f225Degrees_rad}, {"Normalize", false}, {"Angle_deg", 225.0}});
    /* 21 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f225Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 22 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f225Degrees_rad}, {"Normalize", false}, {"Angle_deg", -225.0}});
    /* 23 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f270Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 24 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f270Degrees_rad}, {"Normalize", false}, {"Angle_deg", 270.0}});
    /* 25 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f270Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /* 26 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f270Degrees_rad}, {"Normalize", false}, {"Angle_deg", -270.0}});
    /* 27 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f315Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /* 28 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f315Degrees_rad}, {"Normalize", false}, {"Angle_deg", 315.0}});
    /* 29 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f315Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /* 30 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f315Degrees_rad}, {"Normalize", false}, {"Angle_deg", -315.0}});
    /* 31 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f2PI}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /* 32 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f2PI}, {"Normalize", false}, {"Angle_deg", 360.0}});
    /* 33 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f2PI}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /* 34 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f2PI}, {"Normalize", false}, {"Angle_deg", -360.0}});
    /* 35 */ pTestStep->addDataRow({{"Val", 9*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /* 36 */ pTestStep->addDataRow({{"Val", 9*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 405.0}});
    /* 37 */ pTestStep->addDataRow({{"Val", -9*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /* 38 */ pTestStep->addDataRow({{"Val", -9*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -405.0}});
    /* 39 */ pTestStep->addDataRow({{"Val", 5*ZS::System::Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /* 40 */ pTestStep->addDataRow({{"Val", 5*ZS::System::Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", 450.0}});
    /* 41 */ pTestStep->addDataRow({{"Val", -5*ZS::System::Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 42 */ pTestStep->addDataRow({{"Val", -5*ZS::System::Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", -450.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", 11*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 44 */ pTestStep->addDataRow({{"Val", 11*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 495.0}});
    /* 45 */ pTestStep->addDataRow({{"Val", -11*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 46 */ pTestStep->addDataRow({{"Val", -11*ZS::System::Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -495.0}});
    /* 47 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f3PI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 48 */ pTestStep->addDataRow({{"Val", ZS::System::Math::c_f3PI}, {"Normalize", false}, {"Angle_deg", 540.0}});
    /* 49 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f3PI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 50 */ pTestStep->addDataRow({{"Val", -ZS::System::Math::c_f3PI}, {"Normalize", false}, {"Angle_deg", -540.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Deg2Rad",
        /* strOperation    */ "degree2Rad",
        /* pGrpParent      */ pGrpAngles,
        /* szDoTestStepFct */ SLOT(doTestStepConvertAngles(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", false}, {"Angle_rad", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 45.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f45Degrees_rad}});
    /*  4 */ pTestStep->addDataRow({{"Val", 45.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f45Degrees_rad}});
    /*  5 */ pTestStep->addDataRow({{"Val", -45.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f315Degrees_rad}});
    /*  6 */ pTestStep->addDataRow({{"Val", -45.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f45Degrees_rad}});
    /*  7 */ pTestStep->addDataRow({{"Val", 90.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f90Degrees_rad}});
    /*  8 */ pTestStep->addDataRow({{"Val", 90.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f90Degrees_rad}});
    /*  9 */ pTestStep->addDataRow({{"Val", -90.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f270Degrees_rad}});
    /* 10 */ pTestStep->addDataRow({{"Val", -90.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f90Degrees_rad}});
    /* 11 */ pTestStep->addDataRow({{"Val", 135.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f135Degrees_rad}});
    /* 12 */ pTestStep->addDataRow({{"Val", 135.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f135Degrees_rad}});
    /* 13 */ pTestStep->addDataRow({{"Val", -135.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f225Degrees_rad}});
    /* 14 */ pTestStep->addDataRow({{"Val", -135.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f135Degrees_rad}});
    /* 15 */ pTestStep->addDataRow({{"Val", 180.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_fPI}});
    /* 16 */ pTestStep->addDataRow({{"Val", 180.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_fPI}});
    /* 17 */ pTestStep->addDataRow({{"Val", -180.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_fPI}});
    /* 18 */ pTestStep->addDataRow({{"Val", -180.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_fPI}});
    /* 19 */ pTestStep->addDataRow({{"Val", 225.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f225Degrees_rad}});
    /* 20 */ pTestStep->addDataRow({{"Val", 225.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f225Degrees_rad}});
    /* 21 */ pTestStep->addDataRow({{"Val", -225.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f135Degrees_rad}});
    /* 22 */ pTestStep->addDataRow({{"Val", -225.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f225Degrees_rad}});
    /* 23 */ pTestStep->addDataRow({{"Val", 270.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f270Degrees_rad}});
    /* 24 */ pTestStep->addDataRow({{"Val", 270.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f270Degrees_rad}});
    /* 25 */ pTestStep->addDataRow({{"Val", -270.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f90Degrees_rad}});
    /* 26 */ pTestStep->addDataRow({{"Val", -270.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f270Degrees_rad}});
    /* 27 */ pTestStep->addDataRow({{"Val", 315.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f315Degrees_rad}});
    /* 28 */ pTestStep->addDataRow({{"Val", 315.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f315Degrees_rad}});
    /* 29 */ pTestStep->addDataRow({{"Val", -315.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f45Degrees_rad}});
    /* 30 */ pTestStep->addDataRow({{"Val", -315.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f315Degrees_rad}});
    /* 31 */ pTestStep->addDataRow({{"Val", 360.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /* 32 */ pTestStep->addDataRow({{"Val", 360.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f2PI}});
    /* 33 */ pTestStep->addDataRow({{"Val", -360.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /* 34 */ pTestStep->addDataRow({{"Val", -360.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f2PI}});
    /* 35 */ pTestStep->addDataRow({{"Val", 405.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f45Degrees_rad}});
    /* 36 */ pTestStep->addDataRow({{"Val", 405.0}, {"Normalize", false}, {"Angle_rad", 9.0*ZS::System::Math::c_fPI/4.0}});
    /* 37 */ pTestStep->addDataRow({{"Val", -405.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f315Degrees_rad}});
    /* 38 */ pTestStep->addDataRow({{"Val", -405.0}, {"Normalize", false}, {"Angle_rad", -9.0*ZS::System::Math::c_fPI/4.0}});
    /* 39 */ pTestStep->addDataRow({{"Val", 450.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f90Degrees_rad}});
    /* 40 */ pTestStep->addDataRow({{"Val", 450.0}, {"Normalize", false}, {"Angle_rad", 5.0*ZS::System::Math::c_fPI/2.0}});
    /* 41 */ pTestStep->addDataRow({{"Val", -450.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f270Degrees_rad}});
    /* 42 */ pTestStep->addDataRow({{"Val", -450.0}, {"Normalize", false}, {"Angle_rad", -5.0*ZS::System::Math::c_fPI/2.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", 495.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f135Degrees_rad}});
    /* 44 */ pTestStep->addDataRow({{"Val", 495.0}, {"Normalize", false}, {"Angle_rad", 11.0*ZS::System::Math::c_fPI/4.0}});
    /* 45 */ pTestStep->addDataRow({{"Val", -495.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_f225Degrees_rad}});
    /* 46 */ pTestStep->addDataRow({{"Val", -495.0}, {"Normalize", false}, {"Angle_rad", -11.0*ZS::System::Math::c_fPI/4.0}});
    /* 47 */ pTestStep->addDataRow({{"Val", 540.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_fPI}});
    /* 48 */ pTestStep->addDataRow({{"Val", 540.0}, {"Normalize", false}, {"Angle_rad", ZS::System::Math::c_f3PI}});
    /* 49 */ pTestStep->addDataRow({{"Val", -540.0}, {"Normalize", true}, {"Angle_rad", ZS::System::Math::c_fPI}});
    /* 50 */ pTestStep->addDataRow({{"Val", -540.0}, {"Normalize", false}, {"Angle_rad", -ZS::System::Math::c_f3PI}});

    // Rounding
    //----------

    ZS::Test::CTestStepGroup* pGrpRounding = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Rounding",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2Nearest",
        /* strOperation    */ "round2Nearest",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", 0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", -1}, {"Result", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  5 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  7 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  9 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 5}, {"Result", 0.00002}});
    /* 10 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 5}, {"Result", 0.00001}});
    /* 11 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 5}, {"Result", 0.00002}});
    /* 12 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 5}, {"Result", 0.00007}});
    /* 13 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 5}, {"Result", 0.00006}});
    /* 14 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 5}, {"Result", 0.00007}});
    /* 15 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", -1}, {"Result", 0.000015}});
    /* 16 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", -1}, {"Result", 0.000014}});
    /* 17 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", -1}, {"Result", 0.000016}});
    /* 18 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", -1}, {"Result", 0.000065}});
    /* 19 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", -1}, {"Result", 0.000064}});
    /* 20 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", -1}, {"Result", 0.000066}});
    /* 21 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 22 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 23 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 24 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 25 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 26 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 27 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 5}, {"Result", -0.00002}});
    /* 28 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 5}, {"Result", -0.00001}});
    /* 29 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 5}, {"Result", -0.00002}});
    /* 30 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 5}, {"Result", -0.00007}});
    /* 31 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 5}, {"Result", -0.00006}});
    /* 32 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 5}, {"Result", -0.00007}});
    /* 33 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", -1}, {"Result", -0.000015}});
    /* 34 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", -1}, {"Result", -0.000014}});
    /* 35 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", -1}, {"Result", -0.000016}});
    /* 36 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", -1}, {"Result", -0.000065}});
    /* 37 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", -1}, {"Result", -0.000064}});
    /* 38 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", -1}, {"Result", -0.000066}});
    /* 39 */ pTestStep->addDataRow({{"Val", 1.15}, {"TrailingDigits", 0}, {"Result", 1.0}});
    /* 40 */ pTestStep->addDataRow({{"Val", 1.14}, {"TrailingDigits", 1}, {"Result", 1.1}});
    /* 41 */ pTestStep->addDataRow({{"Val", 1.16}, {"TrailingDigits", 1}, {"Result", 1.2}});
    /* 42 */ pTestStep->addDataRow({{"Val", -1.15}, {"TrailingDigits", 0}, {"Result", -1.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", -1.14}, {"TrailingDigits", 1}, {"Result", -1.1}});
    /* 44 */ pTestStep->addDataRow({{"Val", -1.16}, {"TrailingDigits", 1}, {"Result", -1.2}});
    /* 45 */ pTestStep->addDataRow({{"Val", 1.65}, {"TrailingDigits", 0}, {"Result", 2.0}});
    /* 46 */ pTestStep->addDataRow({{"Val", 1.64}, {"TrailingDigits", 1}, {"Result", 1.6}});
    /* 47 */ pTestStep->addDataRow({{"Val", 1.66}, {"TrailingDigits", 1}, {"Result", 1.7}});
    /* 48 */ pTestStep->addDataRow({{"Val", -1.65}, {"TrailingDigits", 0}, {"Result", -2.0}});
    /* 49 */ pTestStep->addDataRow({{"Val", -1.64}, {"TrailingDigits", 1}, {"Result", -1.6}});
    /* 50 */ pTestStep->addDataRow({{"Val", -1.66}, {"TrailingDigits", 1}, {"Result", -1.7}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2Lower",
        /* strOperation    */ "round2Lower",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", 0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", -1}, {"Result", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  5 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  7 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /*  9 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 5}, {"Result", 0.00001}});
    /* 10 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 5}, {"Result", 0.00001}});
    /* 11 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 5}, {"Result", 0.00001}});
    /* 12 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 5}, {"Result", 0.00006}});
    /* 13 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 5}, {"Result", 0.00006}});
    /* 14 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 5}, {"Result", 0.00006}});
    /* 15 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", -1}, {"Result", 0.000015}});
    /* 16 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", -1}, {"Result", 0.000014}});
    /* 17 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", -1}, {"Result", 0.000016}});
    /* 18 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", -1}, {"Result", 0.000065}});
    /* 19 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", -1}, {"Result", 0.000064}});
    /* 20 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", -1}, {"Result", 0.000066}});
    /* 21 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 22 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 23 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 24 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 25 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 26 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 4}, {"Result", 0.0}});
    /* 27 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 5}, {"Result", -0.00001}});
    /* 28 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 5}, {"Result", -0.00001}});
    /* 29 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 5}, {"Result", -0.00001}});
    /* 30 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 5}, {"Result", -0.00006}});
    /* 31 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 5}, {"Result", -0.00006}});
    /* 32 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 5}, {"Result", -0.00006}});
    /* 33 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", -1}, {"Result", -0.000015}});
    /* 34 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", -1}, {"Result", -0.000014}});
    /* 35 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", -1}, {"Result", -0.000016}});
    /* 36 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", -1}, {"Result", -0.000065}});
    /* 37 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", -1}, {"Result", -0.000064}});
    /* 38 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", -1}, {"Result", -0.000066}});
    /* 39 */ pTestStep->addDataRow({{"Val", 1.15}, {"TrailingDigits", 0}, {"Result", 1.0}});
    /* 40 */ pTestStep->addDataRow({{"Val", 1.14}, {"TrailingDigits", 1}, {"Result", 1.1}});
    /* 41 */ pTestStep->addDataRow({{"Val", 1.16}, {"TrailingDigits", 1}, {"Result", 1.1}});
    /* 42 */ pTestStep->addDataRow({{"Val", -1.15}, {"TrailingDigits", 0}, {"Result", -1.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", -1.14}, {"TrailingDigits", 1}, {"Result", -1.1}});
    /* 44 */ pTestStep->addDataRow({{"Val", -1.16}, {"TrailingDigits", 1}, {"Result", -1.1}});
    /* 45 */ pTestStep->addDataRow({{"Val", 1.65}, {"TrailingDigits", 0}, {"Result", 1.0}});
    /* 46 */ pTestStep->addDataRow({{"Val", 1.64}, {"TrailingDigits", 1}, {"Result", 1.6}});
    /* 47 */ pTestStep->addDataRow({{"Val", 1.66}, {"TrailingDigits", 1}, {"Result", 1.6}});
    /* 48 */ pTestStep->addDataRow({{"Val", -1.65}, {"TrailingDigits", 0}, {"Result", -1.0}});
    /* 49 */ pTestStep->addDataRow({{"Val", -1.64}, {"TrailingDigits", 1}, {"Result", -1.6}});
    /* 50 */ pTestStep->addDataRow({{"Val", -1.66}, {"TrailingDigits", 1}, {"Result", -1.6}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2Upper",
        /* strOperation    */ "round2Upper",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", 0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"TrailingDigits", -1}, {"Result", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  5 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  7 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 4}, {"Result", 0.0001}});
    /*  9 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", 5}, {"Result", 0.00002}});
    /* 10 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", 5}, {"Result", 0.00002}});
    /* 11 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", 5}, {"Result", 0.00002}});
    /* 12 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", 5}, {"Result", 0.00007}});
    /* 13 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", 5}, {"Result", 0.00007}});
    /* 14 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", 5}, {"Result", 0.00007}});
    /* 15 */ pTestStep->addDataRow({{"Val", 0.000015}, {"TrailingDigits", -1}, {"Result", 0.000015}});
    /* 16 */ pTestStep->addDataRow({{"Val", 0.000014}, {"TrailingDigits", -1}, {"Result", 0.000014}});
    /* 17 */ pTestStep->addDataRow({{"Val", 0.000016}, {"TrailingDigits", -1}, {"Result", 0.000016}});
    /* 18 */ pTestStep->addDataRow({{"Val", 0.000065}, {"TrailingDigits", -1}, {"Result", 0.000065}});
    /* 19 */ pTestStep->addDataRow({{"Val", 0.000064}, {"TrailingDigits", -1}, {"Result", 0.000064}});
    /* 20 */ pTestStep->addDataRow({{"Val", 0.000066}, {"TrailingDigits", -1}, {"Result", 0.000066}});
    /* 21 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 22 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 23 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 24 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 25 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 26 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 4}, {"Result", -0.0001}});
    /* 27 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", 5}, {"Result", -0.00002}});
    /* 28 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", 5}, {"Result", -0.00002}});
    /* 29 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", 5}, {"Result", -0.00002}});
    /* 30 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", 5}, {"Result", -0.00007}});
    /* 31 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", 5}, {"Result", -0.00007}});
    /* 32 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", 5}, {"Result", -0.00007}});
    /* 33 */ pTestStep->addDataRow({{"Val", -0.000015}, {"TrailingDigits", -1}, {"Result", -0.000015}});
    /* 34 */ pTestStep->addDataRow({{"Val", -0.000014}, {"TrailingDigits", -1}, {"Result", -0.000014}});
    /* 35 */ pTestStep->addDataRow({{"Val", -0.000016}, {"TrailingDigits", -1}, {"Result", -0.000016}});
    /* 36 */ pTestStep->addDataRow({{"Val", -0.000065}, {"TrailingDigits", -1}, {"Result", -0.000065}});
    /* 37 */ pTestStep->addDataRow({{"Val", -0.000064}, {"TrailingDigits", -1}, {"Result", -0.000064}});
    /* 38 */ pTestStep->addDataRow({{"Val", -0.000066}, {"TrailingDigits", -1}, {"Result", -0.000066}});
    /* 39 */ pTestStep->addDataRow({{"Val", 1.15}, {"TrailingDigits", 0}, {"Result", 2.0}});
    /* 40 */ pTestStep->addDataRow({{"Val", 1.14}, {"TrailingDigits", 1}, {"Result", 1.2}});
    /* 41 */ pTestStep->addDataRow({{"Val", 1.16}, {"TrailingDigits", 1}, {"Result", 1.2}});
    /* 42 */ pTestStep->addDataRow({{"Val", -1.15}, {"TrailingDigits", 0}, {"Result", -2.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", -1.14}, {"TrailingDigits", 1}, {"Result", -1.2}});
    /* 44 */ pTestStep->addDataRow({{"Val", -1.16}, {"TrailingDigits", 1}, {"Result", -1.2}});
    /* 45 */ pTestStep->addDataRow({{"Val", 1.65}, {"TrailingDigits", 0}, {"Result", 2.0}});
    /* 46 */ pTestStep->addDataRow({{"Val", 1.64}, {"TrailingDigits", 1}, {"Result", 1.7}});
    /* 47 */ pTestStep->addDataRow({{"Val", 1.66}, {"TrailingDigits", 1}, {"Result", 1.7}});
    /* 48 */ pTestStep->addDataRow({{"Val", -1.65}, {"TrailingDigits", 0}, {"Result", -2.0}});
    /* 49 */ pTestStep->addDataRow({{"Val", -1.64}, {"TrailingDigits", 1}, {"Result", -1.7}});
    /* 50 */ pTestStep->addDataRow({{"Val", -1.66}, {"TrailingDigits", 1}, {"Result", -1.7}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2Resolution",
        /* strOperation    */ "round2Resolution",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Res", 0.0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.000015}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 0.000014}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.000016}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /*  5 */ pTestStep->addDataRow({{"Val", 0.000065}, {"Resolution", 0.0001}, {"Result", 0.0001}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.000064}, {"Resolution", 0.0001}, {"Result", 0.0001}});
    /*  7 */ pTestStep->addDataRow({{"Val", 0.000066}, {"Resolution", 0.0001}, {"Result", 0.0001}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.000015}, {"Resolution", 0.00001}, {"Result", 0.00002}});
    /*  9 */ pTestStep->addDataRow({{"Val", 0.000014}, {"Resolution", 0.00001}, {"Result", 0.00001}});
    /* 10 */ pTestStep->addDataRow({{"Val", 0.000016}, {"Resolution", 0.00001}, {"Result", 0.00002}});
    /* 11 */ pTestStep->addDataRow({{"Val", 0.000065}, {"Resolution", 0.00001}, {"Result", 0.00007}});
    /* 12 */ pTestStep->addDataRow({{"Val", 0.000064}, {"Resolution", 0.00001}, {"Result", 0.00006}});
    /* 13 */ pTestStep->addDataRow({{"Val", 0.000066}, {"Resolution", 0.00001}, {"Result", 0.00007}});
    /* 14 */ pTestStep->addDataRow({{"Val", -0.000015}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /* 15 */ pTestStep->addDataRow({{"Val", -0.000014}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /* 16 */ pTestStep->addDataRow({{"Val", -0.000016}, {"Resolution", 0.0001}, {"Result", 0.0}});
    /* 17 */ pTestStep->addDataRow({{"Val", -0.000065}, {"Resolution", 0.0001}, {"Result", -0.0001}});
    /* 18 */ pTestStep->addDataRow({{"Val", -0.000064}, {"Resolution", 0.0001}, {"Result", -0.0001}});
    /* 19 */ pTestStep->addDataRow({{"Val", -0.000066}, {"Resolution", 0.0001}, {"Result", -0.0001}});
    /* 20 */ pTestStep->addDataRow({{"Val", -0.000015}, {"Resolution", 0.00001}, {"Result", -0.00002}});
    /* 21 */ pTestStep->addDataRow({{"Val", -0.000014}, {"Resolution", 0.00001}, {"Result", -0.00001}});
    /* 22 */ pTestStep->addDataRow({{"Val", -0.000016}, {"Resolution", 0.00001}, {"Result", -0.00002}});
    /* 23 */ pTestStep->addDataRow({{"Val", -0.000065}, {"Resolution", 0.00001}, {"Result", -0.00007}});
    /* 24 */ pTestStep->addDataRow({{"Val", -0.000064}, {"Resolution", 0.00001}, {"Result", -0.00006}});
    /* 25 */ pTestStep->addDataRow({{"Val", -0.000066}, {"Resolution", 0.00001}, {"Result", -0.00007}});
    /* 26 */ pTestStep->addDataRow({{"Val", 5.15}, {"Resolution", 0.0}, {"Result", 5.15}});
    /* 27 */ pTestStep->addDataRow({{"Val", 5.15}, {"Resolution", 10.0}, {"Result", 10.0}});
    /* 28 */ pTestStep->addDataRow({{"Val", 5.44}, {"Resolution", 0.1}, {"Result", 5.4}});
    /* 29 */ pTestStep->addDataRow({{"Val", 5.45}, {"Resolution", 0.1}, {"Result", 5.5}});
    /* 30 */ pTestStep->addDataRow({{"Val", 5.46}, {"Resolution", 0.1}, {"Result", 5.5}});
    /* 31 */ pTestStep->addDataRow({{"Val", 5.55}, {"Resolution", 0.1}, {"Result", 5.6}});
    /* 32 */ pTestStep->addDataRow({{"Val", 5.65}, {"Resolution", 0.1}, {"Result", 5.7}});
    /* 33 */ pTestStep->addDataRow({{"Val", 5.44}, {"Resolution", 0.01}, {"Result", 5.44}});
    /* 34 */ pTestStep->addDataRow({{"Val", 5.45}, {"Resolution", 0.01}, {"Result", 5.45}});
    /* 35 */ pTestStep->addDataRow({{"Val", 5.46}, {"Resolution", 0.01}, {"Result", 5.46}});
    /* 36 */ pTestStep->addDataRow({{"Val", 5.55}, {"Resolution", 0.01}, {"Result", 5.55}});
    /* 37 */ pTestStep->addDataRow({{"Val", 5.65}, {"Resolution", 0.01}, {"Result", 5.65}});
    /* 38 */ pTestStep->addDataRow({{"Val", 7.33}, {"Resolution", 0.4}, {"Result", 7.2}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2LowerDecade",
        /* strOperation    */ "round2LowerDecade",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.01}, {"Result", 0.01}});
    /*  3 */ pTestStep->addDataRow({{"Val", -0.01}, {"Result", -0.01}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.056734}, {"Result", 0.01}});
    /*  5 */ pTestStep->addDataRow({{"Val", -0.056734}, {"Result", -0.01}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.1}, {"Result", 0.1}});
    /*  7 */ pTestStep->addDataRow({{"Val", -0.1}, {"Result", -0.1}});
    /*  8 */ pTestStep->addDataRow({{"Val", 10.0}, {"Result", 10.0}});
    /*  9 */ pTestStep->addDataRow({{"Val", -10.0}, {"Result", -10.0}});
    /* 10 */ pTestStep->addDataRow({{"Val", 56734.45}, {"Result", 10000.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", -56734.45}, {"Result", -10000.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2UpperDecade",
        /* strOperation    */ "round2UpperDecade",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.01}, {"Result", 0.01}});
    /*  3 */ pTestStep->addDataRow({{"Val", -0.01}, {"Result", -0.01}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.056734}, {"Result", 0.1}});
    /*  5 */ pTestStep->addDataRow({{"Val", -0.056734}, {"Result", -0.1}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.1}, {"Result", 0.1}});
    /*  7 */ pTestStep->addDataRow({{"Val", -0.1}, {"Result", -0.1}});
    /*  8 */ pTestStep->addDataRow({{"Val", 10.0}, {"Result", 10.0}});
    /*  9 */ pTestStep->addDataRow({{"Val", -10.0}, {"Result", -10.0}});
    /* 10 */ pTestStep->addDataRow({{"Val", 56734.45}, {"Result", 100000.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", -56734.45}, {"Result", -100000.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2LowerIntMultOfTen",
        /* strOperation    */ "round2LowerIntMultOfTen",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.01}, {"Result", 0.01}});
    /*  3 */ pTestStep->addDataRow({{"Val", -0.01}, {"Result", -0.01}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.054734}, {"Result", 0.05}});
    /*  5 */ pTestStep->addDataRow({{"Val", -0.054734}, {"Result", -0.05}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.056734}, {"Result", 0.05}});
    /*  7 */ pTestStep->addDataRow({{"Val", -0.056734}, {"Result", -0.05}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.1}, {"Result", 0.1}});
    /*  9 */ pTestStep->addDataRow({{"Val", -0.1}, {"Result", -0.1}});
    /* 10 */ pTestStep->addDataRow({{"Val", 10.0}, {"Result", 10.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", -10.0}, {"Result", -10.0}});
    /* 12 */ pTestStep->addDataRow({{"Val", 54734.45}, {"Result", 50000.0}});
    /* 13 */ pTestStep->addDataRow({{"Val", -54734.45}, {"Result", -50000.0}});
    /* 14 */ pTestStep->addDataRow({{"Val", 56734.45}, {"Result", 50000.0}});
    /* 15 */ pTestStep->addDataRow({{"Val", -56734.45}, {"Result", -50000.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Round2UpperIntMultOfTen",
        /* strOperation    */ "round2UpperIntMultOfTen",
        /* pGrpParent      */ pGrpRounding,
        /* szDoTestStepFct */ SLOT(doTestStepRounding(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Result", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.01}, {"Result", 0.01}});
    /*  3 */ pTestStep->addDataRow({{"Val", -0.01}, {"Result", -0.01}});
    /*  4 */ pTestStep->addDataRow({{"Val", 0.054734}, {"Result", 0.06}});
    /*  5 */ pTestStep->addDataRow({{"Val", -0.054734}, {"Result", -0.06}});
    /*  6 */ pTestStep->addDataRow({{"Val", 0.056734}, {"Result", 0.06}});
    /*  7 */ pTestStep->addDataRow({{"Val", -0.056734}, {"Result", -0.06}});
    /*  8 */ pTestStep->addDataRow({{"Val", 0.1}, {"Result", 0.1}});
    /*  9 */ pTestStep->addDataRow({{"Val", -0.1}, {"Result", -0.1}});
    /* 10 */ pTestStep->addDataRow({{"Val", 10.0}, {"Result", 10.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", -10.0}, {"Result", -10.0}});
    /* 12 */ pTestStep->addDataRow({{"Val", 54734.45}, {"Result", 60000.0}});
    /* 13 */ pTestStep->addDataRow({{"Val", -54734.45}, {"Result", -60000.0}});
    /* 14 */ pTestStep->addDataRow({{"Val", 56734.45}, {"Result", 60000.0}});
    /* 15 */ pTestStep->addDataRow({{"Val", -56734.45}, {"Result", -60000.0}});

    // Min/Max
    //----------

    ZS::Test::CTestStepGroup* pGrpMinMax = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "MinMax",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "AbsMinMax",
        /* strOperation    */ "getAbsMinMax",
        /* pGrpParent      */ pGrpMinMax,
        /* szDoTestStepFct */ SLOT(doTestStepMinMax(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"0", 0.01}, {"1", -0.02}, {"2", 100.0}, {"3", 1.0e3}, {"4", -1.0e4}, {"Min", 0.01}, {"Max", 1.0e4}});

    // FirstSignificantDigit
    //----------------------

    ZS::Test::CTestStepGroup* pGrpFirstSignificantDigit = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "FirstSignificantDigit",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "FirstSignificantDigit",
        /* strOperation    */ "getFirstSignificantDigit",
        /* pGrpParent      */ pGrpFirstSignificantDigit,
        /* szDoTestStepFct */ SLOT(doTestStepFirstSignificantDigit(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({{"Val", 0.0}, {"Result", 0}});
    pTestStep->addDataRow({{"Val", 2.0}, {"Result", 1}});
    pTestStep->addDataRow({{"Val", 10.0}, {"Result", 2}});
    pTestStep->addDataRow({{"Val", 3452.78}, {"Result", 4}});
    pTestStep->addDataRow({{"Val", 0.1}, {"Result", -1}});
    pTestStep->addDataRow({{"Val", 0.5}, {"Result", -1}});
    pTestStep->addDataRow({{"Val", 0.02}, {"Result", -2}});
    pTestStep->addDataRow({{"Val", 0.00056}, {"Result", -4}});

    // Division Lines
    //---------------

    ZS::Test::CTestStepGroup* pGrpDivLines = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Scale Division Lines",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "GetDivLines4LinSpacing-Pixels",
        /* strOperation    */ "getDivLines4LinSpacing",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestDivLines4LinSpacing(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 0.0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 0}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 1.0}, {"DivLineDistPix", 1.0},
        {"DivLinesCount", 0}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 10}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 10.0},
        {"DivLinesCount", 2}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 10}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 10.0},
        {"DivLinesCount", 3}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 999.0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 50.0}, {"DivLineDistPix", 50.0},
        {"DivLinesCount", 20}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 1000.0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 50.0}, {"DivLineDistPix", 50.0},
        {"DivLinesCount", 21}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "GetDivLines4LinSpacing-WorldCoors",
        /* strOperation    */ "getDivLines4LinSpacing",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestDivLines4LinSpacing(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 0.0}, {"ScaleRangePix", 0},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 0}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 0.0}, {"DivLineDistPix", 0.0},
        {"DivLinesCount", 0}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRangePix", 20},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 10}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 19.0},
        {"DivLinesCount", 2}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRangePix", 21},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 10}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 5.0}, {"DivLineDistPix", 10.0},
        {"DivLinesCount", 3}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 100.0}, {"ScaleRangePix", 1000},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 99.9},
        {"DivLinesCount", 11}});
    pTestStep->addDataRow({
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 100.0}, {"ScaleRangePix", 1001},
        {"DivLineDistMinVal", 0.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 5.0}, {"DivLineDistPix", 50.0},
        {"DivLinesCount", 21}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "DivisionLineValues-Pixels",
        /* strOperation    */ "getValues",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepDivLinesGetVal(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10}, {"DivLineInPix", 10},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 5.0}, {"ValExpected", 5.0},
        {"Val", 5.0}, {"ValInPixExpected", 5.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10}, {"DivLineInPix", 10},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 10.0}, {"ValExpected", 10.0},
        {"Val", 10.0}, {"ValInPixExpected", 10.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10}, {"DivLineInPix", 10},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 15.0}, {"ValExpected", 15.0},
        {"Val", 15.0}, {"ValInPixExpected", 15.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10}, {"DivLineInPix", 10},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 20.0}, {"ValExpected", 20.0},
        {"Val", 20.0}, {"ValInPixExpected", 20.0}});

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10}, {"DivLineInPix", 10},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 5.0}, {"ValExpected", 5.0},
        {"Val", 5.0}, {"ValInPixExpected", 5.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 2}, {"DivLineVal", 20}, {"DivLineInPix", 20},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 10.0}, {"ValExpected", 10.0},
        {"Val", 10.0}, {"ValInPixExpected", 10.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 2}, {"DivLineVal", 20}, {"DivLineInPix", 20},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 15.0}, {"ValExpected", 15.0},
        {"Val", 15.0}, {"ValInPixExpected", 15.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", false},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 2}, {"DivLineVal", 20}, {"DivLineInPix", 20},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 20.0}, {"ValExpected", 20.0},
        {"Val", 20.0}, {"ValInPixExpected", 20.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "DivisionLineValues-WorldCoors-XScale",
        /* strOperation    */ "getValues",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepDivLinesGetVal(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 19},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 5.0}, {"ValExpected", 2.6},
        {"Val", 5.0}, {"ValInPixExpected", 9.5}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 19},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 10.0}, {"ValExpected", 5.3},
        {"Val", 10.0}, {"ValInPixExpected", 19.0}});

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 1}, {"DivLineVal", 5.0}, {"DivLineInPix", 10},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 5.0}, {"ValExpected", 2.5},
        {"Val", 5.0}, {"ValInPixExpected", 10.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::X).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 2}, {"DivLineVal", 10.0}, {"DivLineInPix", 20},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 10.0}, {"ValExpected", 5.0},
        {"Val", 10.0}, {"ValInPixExpected", 20.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "DivisionLineValues-WorldCoors-YScale-BottomUp",
        /* strOperation    */ "getValues",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepDivLinesGetVal(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 19.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 19},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", -19.0},
        {"ValInPix", 0.0}, {"ValExpected", 10.0},
        {"Val", 0.0}, {"ValInPixExpected", 19.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 19.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", -19.0},
        {"ValInPix", 5.0}, {"ValExpected", 7.4},
        {"Val", 5.0}, {"ValInPixExpected", 9.5}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 19.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", -19.0},
        {"ValInPix", 10.0}, {"ValExpected", 4.7},
        {"Val", 10.0}, {"ValInPixExpected", 0.0}});

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 20.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 20},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", -10.0},
        {"ValInPix", 0.0}, {"ValExpected", 10.0},
        {"Val", 0.0}, {"ValInPixExpected", 20.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 20.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 1}, {"DivLineVal", 5.0}, {"DivLineInPix", 10},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", -10.0},
        {"ValInPix", 5.0}, {"ValExpected", 7.5},
        {"Val", 5.0}, {"ValInPixExpected", 10.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 20.0}, {"ScaleMaxInPix", 0.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 2}, {"DivLineVal", 10.0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", -10.0},
        {"ValInPix", 10.0}, {"ValExpected", 5.0},
        {"Val", 10.0}, {"ValInPixExpected", 0.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "DivisionLineValues-WorldCoors-YScale-TopDown",
        /* strOperation    */ "getValues",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepDivLinesGetVal(ZS::Test::CTestStep*)) );

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 19},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 5.0}, {"ValExpected", 2.6},
        {"Val", 5.0}, {"ValInPixExpected", 9.5}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 19.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 2}, {"DivLinesDistMin", 10},
        {"IdxDivLine", 1}, {"DivLineVal", 10.0}, {"DivLineInPix", 19},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 10.0}, {"DivLineDistInPix", 19.0},
        {"ValInPix", 10.0}, {"ValExpected", 5.3},
        {"Val", 10.0}, {"ValInPixExpected", 19.0}});

    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 0}, {"DivLineVal", 0}, {"DivLineInPix", 0},
        {"IdxDivLine1", 0}, {"IdxDivLine2", 1}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 0.0}, {"ValExpected", 0.0},
        {"Val", 0.0}, {"ValInPixExpected", 0.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 1}, {"DivLineVal", 5.0}, {"DivLineInPix", 10},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 5.0}, {"ValExpected", 2.5},
        {"Val", 5.0}, {"ValInPixExpected", 10.0}});
    pTestStep->addDataRow({
        {"ScaleAxis", CEnumScaleAxis(EScaleAxis::Y).toString()},
        {"Spacing", CEnumSpacing(ESpacing::Linear).toString()},
        {"UseWorldTransformation", true},
        {"ScaleMinVal", 0.0}, {"ScaleMaxVal", 10.0}, {"ScaleRes", 0.1},
        {"ScaleMinInPix", 0.0}, {"ScaleMaxInPix", 20.0}, {"DivLineDistMinPix", 10},
        {"DivLineLayer", CEnumDivLineLayer(EDivLineLayer::Main).toString()},
        {"DivLinesCount", 3}, {"DivLinesDistMin", 5},
        {"IdxDivLine", 2}, {"DivLineVal", 10.0}, {"DivLineInPix", 20},
        {"IdxDivLine1", 1}, {"IdxDivLine2", 2}, {"DivLineDistVal", 5.0}, {"DivLineDistInPix", 10.0},
        {"ValInPix", 10.0}, {"ValExpected", 5.0},
        {"Val", 10.0}, {"ValInPixExpected", 20.0}});

    // Merge Arrays
    //---------------

    ZS::Test::CTestStepGroup* pGrpMergeArrays = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Merge Arrays",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Merge",
        /* strOperation    */ "merge",
        /* pGrpParent      */ pGrpMergeArrays,
        /* szDoTestStepFct */ SLOT(doTestStepMergeArrays(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"ValCountArr1", 0},
        {"ValCountArr2", 0},
        {"ValCountArrMerged", 0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 1},
            {"ValArr1[0]", 0.0},
        {"ValCountArr2", 0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 5},
            {"ValArr1[0]", 0.0}, {"ValArr1[1]", 0.0}, {"ValArr1[2]", 0.0}, {"ValArr1[3]", 0.0}, {"ValArr1[4]", 0.0},
        {"ValCountArr2", 0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 5},
            {"ValArr1[0]", 0.0}, {"ValArr1[1]", 0.5}, {"ValArr1[2]", 1.0}, {"ValArr1[3]", 1.5}, {"ValArr1[4]", 2.0},
        {"ValCountArr2", 0},
        {"ValCountArrMerged", 5},
            {"ValArrMerged[0]", 0.0}, {"ValArrMerged[1]", 0.5}, {"ValArrMerged[2]", 1.0}, {"ValArrMerged[3]", 1.5}, {"ValArrMerged[4]", 2.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 0},
        {"ValCountArr2", 1},
            {"VarArr2[0]", 0.0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 0},
        {"ValCountArr2", 5},
            {"VarArr2[0]", 0.0}, {"VarArr2[1]", 0.0}, {"VarArr2[2]", 0.0}, {"VarArr2[3]", 0.0}, {"VarArr2[4]", 0.0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 0},
        {"ValCountArr2", 5},
            {"VarArr2[0]", 0.0}, {"VarArr2[1]", 0.5}, {"VarArr2[2]", 1.0}, {"VarArr2[3]", 1.5}, {"VarArr2[4]", 2.0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}, {"ValArrMerged[1]", 0.5}, {"ValArrMerged[2]", 1.0}, {"ValArrMerged[3]", 1.5}, {"ValArrMerged[4]", 2.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 1},
            {"ValArr1[0]", 0.0},
        {"ValCountArr2", 1},
            {"VarArr2[0]", 0.0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 1},
            {"ValArr1[0]", 1.0},
        {"ValCountArr2", 1},
            {"VarArr2[0]", 0.5},
        {"ValCountArrMerged", 2},
            {"ValArrMerged[0]", 0.5}, {"ValArrMerged[1]", 1.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 5},
            {"ValArr1[0]", 0.0}, {"ValArr1[1]", 0.0}, {"ValArr1[2]", 0.0}, {"ValArr1[3]", 0.0}, {"ValArr1[4]", 0.0},
        {"ValCountArr2", 5},
            {"VarArr2[0]", 0.0}, {"VarArr2[1]", 0.0}, {"VarArr2[2]", 0.0}, {"VarArr2[3]", 0.0}, {"VarArr2[4]", 0.0},
        {"ValCountArrMerged", 1},
            {"ValArrMerged[0]", 0.0}
        });
    pTestStep->addDataRow({
        {"ValCountArr1", 5},
            {"ValArr1[0]", -2.0}, {"ValArr1[1]", -1.0}, {"ValArr1[2]", 0.0}, {"ValArr1[3]", 1.0}, {"ValArr1[4]", 2.0},
        {"ValCountArr2", 30},
            {"VarArr2[0]", -3.0}, {"VarArr2[1]", -2.8}, {"VarArr2[2]", -2.6}, {"VarArr2[3]", -2.4}, {"VarArr2[4]", -2.2},
            {"VarArr2[5]", -2.0}, {"VarArr2[6]", -1.8}, {"VarArr2[7]", -1.6}, {"VarArr2[8]", -1.6}, {"VarArr2[9]", -1.2},
            {"VarArr2[10]", -1.0}, {"VarArr2[11]", -0.8}, {"VarArr2[12]", -0.6}, {"VarArr2[13]", -0.6}, {"VarArr2[14]", -0.2},
            {"VarArr2[15]", 0.0}, {"VarArr2[16]", 0.2}, {"VarArr2[17]", 0.4}, {"VarArr2[18]", 0.6}, {"VarArr2[19]", 0.8},
            {"VarArr2[20]", 1.0}, {"VarArr2[21]", 1.2}, {"VarArr2[22]", 1.4}, {"VarArr2[23]", 1.6}, {"VarArr2[24]", 1.8},
            {"VarArr2[25]", 2.0}, {"VarArr2[26]", 2.2}, {"VarArr2[27]", 2.4}, {"VarArr2[28]", 2.6}, {"VarArr2[29]", 2.8},
        {"ValCountArrMerged", 30},
            {"ValArrMerged[0]", -3.0}, {"ValArrMerged[1]", -2.8}, {"ValArrMerged[2]", -2.6}, {"ValArrMerged[3]", -2.4}, {"ValArrMerged[4]", -2.2},
            {"ValArrMerged[5]", -2.0}, {"ValArrMerged[6]", -1.8}, {"ValArrMerged[7]", -1.6}, {"ValArrMerged[8]", -1.6}, {"ValArrMerged[9]", -1.2},
            {"ValArrMerged[10]", -1.0}, {"ValArrMerged[11]", -0.8}, {"ValArrMerged[12]", -0.6}, {"ValArrMerged[13]", -0.6}, {"ValArrMerged[14]", -0.2},
            {"ValArrMerged[15]", 0.0}, {"ValArrMerged[16]", 0.2}, {"ValArrMerged[17]", 0.4}, {"ValArrMerged[18]", 0.6}, {"ValArrMerged[19]", 0.8},
            {"ValArrMerged[20]", 1.0}, {"ValArrMerged[21]", 1.2}, {"ValArrMerged[22]", 1.4}, {"ValArrMerged[23]", 1.6}, {"ValArrMerged[24]", 1.8},
            {"ValArrMerged[25]", 2.0}, {"ValArrMerged[26]", 2.2}, {"ValArrMerged[27]", 2.4}, {"ValArrMerged[28]", 2.6}, {"ValArrMerged[29]", 2.8},
        });

    // Merge Arrays
    //---------------

    ZS::Test::CTestStepGroup* pGrpPrecision = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Precision",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "GetPrecision2ShowUniqueNumbers",
        /* strOperation    */ "getPrecision2ShowUniqueNumbers",
        /* pGrpParent      */ pGrpPrecision,
        /* szDoTestStepFct */ SLOT(doTestStepPrecision2ShowUniqueNumbers(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"ValCount", 0},
        {"ExponentDigits", 0},
        {"PrecisionMin", 1},
        {"PrecisionMax", 10},
        {"Precision", 1}
        });
    pTestStep->addDataRow({
        {"ValCount", 5},
            {"ValArr[0]", -2.11}, {"ValArr[1]", -1.0}, {"ValArr[2]", 2.11}, {"ValArr[3]", 2.111}, {"ValArr[4]", 2.1111},
        {"ExponentDigits", 0},
        {"PrecisionMin", 1},
        {"PrecisionMax", 10},
        {"Precision", 4}
        });

    // Recall test step settings
    //--------------------------

    recallTestSteps();

} // default ctor

//------------------------------------------------------------------------------
CTest::~CTest()
//------------------------------------------------------------------------------
{
    SErrResultInfo errResultInfo = saveTestSteps();

    if(errResultInfo.isErrorResult())
    {
        if(CErrLog::GetInstance() != nullptr)
        {
            CErrLog::GetInstance()->addEntry(errResultInfo);
        }
    }

} // dtor

/*==============================================================================
public slots:
==============================================================================*/

//------------------------------------------------------------------------------
void CTest::doTestStepConvertAngles( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double fVal = 0.0;
        bool bNormalize = true;
        double fResult = 0.0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "Val") {
                fVal = dataRow["Val"].toDouble();
            }
            else if (strKey == "Normalize") {
                bNormalize = dataRow["Normalize"].toBool();
            }
            else if (strKey == "Angle_deg") {
                fResult = dataRow["Angle_deg"].toDouble();
                mapExpectedValues[strKey] = QString::number(fResult);
            }
            else if (strKey == "Angle_rad") {
                fResult = dataRow["Angle_rad"].toDouble();
                mapExpectedValues[strKey] = QString::number(fResult);
            }
        }

        if( strOperation.compare("rad2Degree") == 0)
        {
            fResult = ZS::System::Math::rad2Degree(fVal, bNormalize);
            mapResultValues["Angle_deg"] = QString::number(fResult);
        }
        else if( strOperation.compare("degree2Rad") == 0)
        {
            fResult = ZS::System::Math::degree2Rad(fVal, bNormalize);
            mapResultValues["Angle_rad"] = QString::number(fResult);
        }

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepConvertAngles

//------------------------------------------------------------------------------
void CTest::doTestStepRounding( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double fVal = 0.0;
        int iTrailingDigits = 0;
        double fResolution = 0.0;
        double fResult = 0.0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "Val") {
                fVal = dataRow["Val"].toDouble();
            }
            else if (strKey == "TrailingDigits") {
                iTrailingDigits = dataRow["TrailingDigits"].toInt();
            }
            else if (strKey == "Resolution") {
                fResolution = dataRow["Resolution"].toDouble();
            }
            else if (strKey == "Result") {
                fResult = dataRow["Result"].toDouble();
                mapExpectedValues[strKey] = QString::number(fResult);
            }
        }

        if (strOperation.compare("round2Nearest") == 0)
        {
            fResult = ZS::System::Math::round2Nearest(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Lower") == 0)
        {
            fResult = ZS::System::Math::round2Lower(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Upper") == 0)
        {
            fResult = ZS::System::Math::round2Upper(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Resolution") == 0)
        {
            fResult = ZS::System::Math::round2Resolution(fVal, fResolution);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2LowerDecade") == 0)
        {
            fResult = ZS::System::Math::round2LowerDecade(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2UpperDecade") == 0)
        {
            fResult = ZS::System::Math::round2UpperDecade(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2LowerIntMultOfTen") == 0)
        {
            fResult = ZS::System::Math::round2LowerIntMultOfTen(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2UpperIntMultOfTen") == 0)
        {
            fResult = ZS::System::Math::round2UpperIntMultOfTen(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepRounding

//------------------------------------------------------------------------------
void CTest::doTestStepMinMax( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double  fMin = 0.0;
        double  fMax = 0.0;
        QVector<double> arfVals;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "Min") {
                fMin = dataRow["Min"].toDouble();
                mapExpectedValues[strKey] = QString::number(fMin);
            }
            else if (strKey == "Max") {
                fMax = dataRow["Max"].toDouble();
                mapExpectedValues[strKey] = QString::number(fMax);
            }
            else
            {
                arfVals.append(dataRow[strKey].toDouble());
            }
        }

        if (strOperation.compare("getAbsMinMax") == 0)
        {
            std::tuple<double, double> minMax = ZS::System::Math::getAbsMinMax(arfVals);
            mapResultValues["Min"] = QString::number(std::get<0>(minMax));
            mapResultValues["Max"] = QString::number(std::get<1>(minMax));
        }

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMinMax

//------------------------------------------------------------------------------
void CTest::doTestStepFirstSignificantDigit( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    QString strOperation = i_pTestStep->getOperation();

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double  fVal = 0.0;
        int iDigit = 0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "Val") {
                fVal = dataRow["Val"].toDouble();
            }
            else if (strKey == "Result") {
                iDigit = dataRow["Result"].toDouble();
                mapExpectedValues[strKey] = QString::number(iDigit);
            }
        }

        if (strOperation.compare("getFirstSignificantDigit") == 0)
        {
            iDigit = ZS::System::Math::getFirstSignificantDigit(fVal);
            mapResultValues["Result"] = QString::number(iDigit);
        }

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepFirstSignificantDigit

//------------------------------------------------------------------------------
void CTest::doTestDivLines4LinSpacing( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        bool   bUseWorldTransformation = false;
        double fScaleMinVal = 0.0;
        double fScaleMaxVal = 0.0;
        int    iScaleRangePix = 0;
        double fDivLineDistMinVal = 0.0;
        int    iDivLineDistMinPix = 0;
        bool   bUseDivLineDistValDecimalFactor25 = false;
        double fDivLineFirstValExpected = 0.0;
        double fDivLineDistFirstPixExpected = 0.0;
        double fDivLineDistValExpected = 0.0;
        double fDivLineDistPixExpected = 0.0;
        int    iDivLineCountExpected = 0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "UseWorldTransformation") {
                bUseWorldTransformation = dataRow["UseWorldTransformation"].toBool();
            }
            else if (strKey == "ScaleMinVal") {
                fScaleMinVal = dataRow["ScaleMinVal"].toDouble();
            }
            else if (strKey == "ScaleMaxVal") {
                fScaleMaxVal = dataRow["ScaleMaxVal"].toDouble();
            }
            else if (strKey == "ScaleRangePix") {
                iScaleRangePix = dataRow["ScaleRangePix"].toInt();
            }
            else if (strKey == "DivLineDistMinVal") {
                fDivLineDistMinVal = dataRow["DivLineDistMinVal"].toDouble();
            }
            else if (strKey == "DivLineDistMinPix") {
                iDivLineDistMinPix = dataRow["DivLineDistMinPix"].toInt();
            }
            else if (strKey == "UseDivLineDistValDecimalFactor25") {
                bUseDivLineDistValDecimalFactor25 = dataRow["UseDivLineDistValDecimalFactor25"].toBool();
            }
            else if (strKey == "DivLineFirstVal") {
                fDivLineFirstValExpected = dataRow["DivLineFirstVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineFirstVal"].toString();
            }
            else if (strKey == "DivLineDistFirstPix") {
                fDivLineDistFirstPixExpected = dataRow["DivLineDistFirstPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistFirstPix"].toString();
            }
            else if (strKey == "DivLineDistVal") {
                fDivLineDistValExpected = dataRow["DivLineDistVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistVal"].toString();
            }
            else if (strKey == "DivLineDistPix") {
                fDivLineDistPixExpected = dataRow["DivLineDistPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistPix"].toString();
            }
            else if (strKey == "DivLinesCount") {
                iDivLineCountExpected = dataRow["DivLinesCount"].toInt();
                mapExpectedValues[strKey] = dataRow["DivLinesCount"].toString();
            }
        }

        int iDivLineCountResult = -1;
        double fDivLineFirstValResult = 0.0;
        double fDivLineDistFirstPixResult = 0.0;
        double fDivLineDistValResult = 0.0;
        double fDivLineDistPixResult = 0.0;

        if (bUseWorldTransformation) {
            iDivLineCountResult = ZS::System::Math::CScaleDivLines::getDivLines4LinSpacing(
                /* fScaleMinVal          */ fScaleMinVal,
                /* fScaleMaxVal          */ fScaleMaxVal,
                /* iScaleRangePix        */ iScaleRangePix,
                /* fDivLineDistMinVal    */ fDivLineDistMinVal,
                /* iDivLineDistMinPix    */ iDivLineDistMinPix,
                /* bUseDivLineFactor25   */ bUseDivLineDistValDecimalFactor25,
                /* pfDivLineFirstVal     */ &fDivLineFirstValResult,
                /* pfDivLineDistFirstPix */ &fDivLineDistFirstPixResult,
                /* pfDivLineDistVal      */ &fDivLineDistValResult,
                /* pfDivLineDistPix      */ &fDivLineDistPixResult );
        }
        else {
            iDivLineCountResult = ZS::System::Math::CScaleDivLines::getDivLines4LinSpacing(
                /* fScaleMinVal          */ fScaleMinVal,
                /* fScaleMaxVal          */ fScaleMaxVal,
                /* fDivLineDistMinVal    */ fDivLineDistMinVal,
                /* iDivLineDistMinPix    */ iDivLineDistMinPix,
                /* bUseDivLineFactor25   */ bUseDivLineDistValDecimalFactor25,
                /* pfDivLineFirstVal     */ &fDivLineFirstValResult,
                /* pfDivLineDistFirstPix */ &fDivLineDistFirstPixResult,
                /* pfDivLineDistVal      */ &fDivLineDistValResult,
                /* pfDivLineDistPix      */ &fDivLineDistPixResult );
        }

        mapResultValues["DivLineFirstVal"] = QString::number(fDivLineFirstValResult);
        mapResultValues["DivLineDistFirstPix"] = QString::number(fDivLineDistFirstPixResult);
        mapResultValues["DivLineDistVal"] = QString::number(fDivLineDistValResult);
        mapResultValues["DivLineDistPix"] = QString::number(fDivLineDistPixResult);
        mapResultValues["DivLinesCount"] = QString::number(iDivLineCountResult);

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestDivLines4LinSpacing

//------------------------------------------------------------------------------
void CTest::doTestStepDivLinesGetVal( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QString strOperation = i_pTestStep->getOperation();

    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        CEnumScaleAxis eScaleAxis = EScaleAxis::X;
        CEnumSpacing eSpacing = ESpacing::Linear;
        bool bUseWorldTransformation = false;
        double fScaleMinVal = 0.0;
        double fScaleMaxVal = 0.0;
        double fScaleRes = 1.0;
        int iScaleMin_px = 0;
        int iScaleMax_px = 0;
        int iDivLineDistMinPix = 0;
        CEnumDivLineLayer eDivLineLayer = EDivLineLayer::Main;
        int idxDivLine = 0;
        int idxDivLine1 = 0;
        int idxDivLine2 = 0;
        double fValInPix = 0.0;
        double fVal = 0.0;

        int iDivLinesCountExpected = 0;
        double fDivLineValExpected = 0.0;
        double fDivLineInPixExpected = 0.0;
        double fDivLinesDistMinExpected = 0.0;
        double fDivLineDistValExpected = 0.0;
        double fDivLineDistPixExpected = 0.0;
        double fValInPixExpected = 0.0;
        double fValExpected = 0.0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "ScaleAxis") {
                eScaleAxis = dataRow["ScaleAxis"].toString();
            }
            else if (strKey == "Spacing") {
                eSpacing = dataRow["Spacing"].toString();
            }
            else if (strKey == "UseWorldTransformation") {
                bUseWorldTransformation = dataRow["UseWorldTransformation"].toBool();
            }
            else if (strKey == "ScaleMinVal") {
                fScaleMinVal = dataRow["ScaleMinVal"].toDouble();
            }
            else if (strKey == "ScaleMaxVal") {
                fScaleMaxVal = dataRow["ScaleMaxVal"].toDouble();
            }
            else if (strKey == "ScaleRes") {
                fScaleRes = dataRow["ScaleRes"].toDouble();
            }
            else if (strKey == "ScaleMinInPix") {
                iScaleMin_px = dataRow["ScaleMinInPix"].toInt();
            }
            else if (strKey == "ScaleMaxInPix") {
                iScaleMax_px = dataRow["ScaleMaxInPix"].toInt();
            }
            else if (strKey == "DivLineDistMinPix") {
                iDivLineDistMinPix = dataRow["DivLineDistMinPix"].toInt();
            }
            else if (strKey == "DivLineLayer") {
                eDivLineLayer = dataRow["DivLineLayer"].toString();
            }
            else if (strKey == "IdxDivLine") {
                idxDivLine = dataRow["IdxDivLine"].toDouble();
            }
            else if (strKey == "IdxDivLine1") {
                idxDivLine1 = dataRow["IdxDivLine1"].toInt();
            }
            else if (strKey == "IdxDivLine2") {
                idxDivLine2 = dataRow["IdxDivLine2"].toInt();
            }
            else if (strKey == "DivLinesCount") {
                iDivLinesCountExpected = dataRow["DivLinesCount"].toInt();
                mapExpectedValues[strKey] = dataRow["DivLinesCount"].toString();
            }
            else if (strKey == "DivLinesDistMin") {
                fDivLinesDistMinExpected = dataRow["DivLinesDistMin"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLinesDistMin"].toString();
            }
            else if (strKey == "DivLineVal") {
                fDivLineValExpected = dataRow["DivLineVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineVal"].toString();
            }
            else if (strKey == "DivLineInPix") {
                fDivLineInPixExpected = dataRow["DivLineInPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineInPix"].toString();
            }
            else if (strKey == "DivLineDistVal") {
                fDivLineDistValExpected = dataRow["DivLineDistVal"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistVal"].toString();
            }
            else if (strKey == "DivLineDistInPix") {
                fDivLineDistPixExpected = dataRow["DivLineDistInPix"].toDouble();
                mapExpectedValues[strKey] = dataRow["DivLineDistInPix"].toString();
            }
            else if (strKey == "ValInPix") {
                fValInPix = dataRow["ValInPix"].toDouble();
            }
            else if (strKey == "ValExpected") {
                fValExpected = dataRow["ValExpected"].toDouble();
                mapExpectedValues["Val"] = dataRow["ValExpected"].toString();
            }
            else if (strKey == "Val") {
                fVal = dataRow["Val"].toDouble();
            }
            else if (strKey == "ValInPixExpected") {
                fValInPixExpected = dataRow["ValInPixExpected"].toDouble();
                mapExpectedValues["ValInPix"] = dataRow["ValInPixExpected"].toString();
            }
        }

        ZS::System::Math::CScaleDivLines divLines(eScaleAxis.enumerator());

        divLines.setSpacing(eSpacing);
        divLines.setUseWorldCoordinateTransformation(bUseWorldTransformation);
        if (bUseWorldTransformation) {
            divLines.setScale(fScaleMinVal, fScaleMaxVal, fScaleRes, iScaleMin_px, iScaleMax_px);
        }
        else {
            divLines.setScale(fScaleMinVal, fScaleMaxVal, fScaleRes);
        }
        divLines.setDivLinesDistMinInPix(eDivLineLayer, iDivLineDistMinPix);

        divLines.update();

        int iDivLinesCountResult = divLines.getDivLinesCount(eDivLineLayer);
        double fDivLinesDistMinResult = divLines.getDivLinesDistMin(eDivLineLayer);
        double fDivLineValResult = divLines.getDivLineVal(eDivLineLayer, idxDivLine);
        double fDivLineInPixResult = divLines.getDivLineInPix(eDivLineLayer, idxDivLine);
        double fDivLineDistValResult = divLines.getDivLineDistVal(eDivLineLayer, idxDivLine1, idxDivLine2);
        double fDivLineDistPixResult = divLines.getDivLineDistInPix(eDivLineLayer, idxDivLine1, idxDivLine2);
        double fValResult = divLines.getVal(fValInPix);
        double fValInPixResult = divLines.getValInPix(fVal);

        mapResultValues["DivLinesCount"] = QString::number(iDivLinesCountResult);
        mapResultValues["DivLinesDistMin"] = QString::number(fDivLinesDistMinResult);
        mapResultValues["DivLineVal"] = QString::number(fDivLineValResult);
        mapResultValues["DivLineInPix"] = QString::number(fDivLineInPixResult);
        mapResultValues["DivLineDistVal"] = QString::number(fDivLineDistValResult);
        mapResultValues["DivLineDistInPix"] = QString::number(fDivLineDistPixResult);
        mapResultValues["Val"] = QString::number(fValResult);
        mapResultValues["ValInPix"] = QString::number(fValInPixResult);

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepDivLinesGetVal

//------------------------------------------------------------------------------
void CTest::doTestStepMergeArrays( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        int iValCountArr1 = dataRow["ValCountArr1"].toInt();
        int iValCountArr2 = dataRow["ValCountArr2"].toInt();
        int iValCountArrMerged = dataRow["ValCountArrMerged"].toInt();

        QVector<double> arfVals1(iValCountArr1, 0.0);
        QVector<double> arfVals2(iValCountArr2, 0.0);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey.startsWith("ValArr1")) {
                QStringList strlst = strKey.split("[");
                strlst[1] = strlst[1].mid(0, strlst[1].length()-1);
                int idxVal = strlst[1].toInt();
                arfVals1[idxVal] = dataRow[strKey].toDouble();
            }
            else if (strKey.startsWith("VarArr2")) {
                QStringList strlst = strKey.split("[");
                strlst[1] = strlst[1].mid(0, strlst[1].length()-1);
                int idxVal = strlst[1].toInt();
                arfVals2[idxVal] = dataRow[strKey].toDouble();
            }
            else if (strKey.startsWith("ValArrMerged")) {
                mapExpectedValues[strKey] = dataRow[strKey].toString();
            }
        }

        QVector<double> arfValsMerged = ZS::System::Math::merge(arfVals1, arfVals2);

        for (int idxVal = 0; idxVal < arfValsMerged.size(); ++idxVal) {
            QString strKey = "ValArrMerged[" + QString::number(idxVal) + "]";
            mapResultValues[strKey] = QString::number(arfValsMerged[idxVal]);
        }

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepMergeArrays

//------------------------------------------------------------------------------
void CTest::doTestStepPrecision2ShowUniqueNumbers( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        int iValCount = dataRow["ValCount"].toInt();
        QVector<double> arfVals(iValCount, 0.0);
        int iExponentDigits = 0;
        int iPrecisionMin = 1;
        int iPrecisionMax = 10;

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey.startsWith("ValArr")) {
                QStringList strlst = strKey.split("[");
                strlst[1] = strlst[1].mid(0, strlst[1].length()-1);
                int idxVal = strlst[1].toInt();
                arfVals[idxVal] = dataRow[strKey].toDouble();
            }
            else if (strKey.startsWith("ExponentDigits")) {
                iExponentDigits = dataRow[strKey].toInt();
            }
            else if (strKey.startsWith("PrecisionMin")) {
                iPrecisionMin = dataRow[strKey].toInt();
            }
            else if (strKey.startsWith("PrecisionMax")) {
                iPrecisionMax = dataRow[strKey].toInt();
            }
            else if (strKey.startsWith("Precision")) {
                mapExpectedValues[strKey] = dataRow[strKey].toString();
            }
        }

        int iPrecision = ZS::System::Math::CScaleDivLines::getPrecision2ShowUniqueNumbers(
            arfVals, iExponentDigits, iPrecisionMin, iPrecisionMax);
        mapResultValues["Precision"] = QString::number(iPrecision);

        QString strExpectedValues;
        for (const QString& strKey : mapExpectedValues.keys())
        {
            if (!strExpectedValues.isEmpty()) strExpectedValues += ", ";
            strExpectedValues += strKey + "=" + mapExpectedValues[strKey];
        }
        strlstExpectedValues.append(strExpectedValues);

        QString strResultValues;
        for (const QString& strKey : mapResultValues.keys())
        {
            if (!strResultValues.isEmpty()) strResultValues += ", ";
            strResultValues += strKey + "=" + mapResultValues[strKey];
        }
        strlstResultValues.append(strResultValues);
    }

    i_pTestStep->setExpectedValues(strlstExpectedValues);
    i_pTestStep->setResultValues(strlstResultValues);

} // doTestStepPrecision2ShowUniqueNumbers
