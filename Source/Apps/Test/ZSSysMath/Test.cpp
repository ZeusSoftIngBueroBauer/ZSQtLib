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
#include "ZSSys/ZSSysTrcAdminObj.h"
#include "ZSSys/ZSSysTrcMethod.h"

#include "ZSSys/ZSSysMemLeakDump.h"


using namespace ZS::System;
using namespace ZS::Apps::Test::Template;


/*******************************************************************************
class CTest
*******************************************************************************/

/*==============================================================================
public: // ctors and dtor
==============================================================================*/

//------------------------------------------------------------------------------
CTest::CTest() :
//------------------------------------------------------------------------------
    ZS::Test::CTest("ZSSysMath")
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
        /* strOperation    */ "rad2Deg",
        /* pGrpParent      */ pGrpAngles,
        /* szDoTestStepFct */ SLOT(doTestStepConvertAngles(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", false}, {"Angle_deg", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /*  4 */ pTestStep->addDataRow({{"Val", Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 45.0}});
    /*  5 */ pTestStep->addDataRow({{"Val", -Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /*  6 */ pTestStep->addDataRow({{"Val", -Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -45.0}});
    /*  7 */ pTestStep->addDataRow({{"Val", Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /*  8 */ pTestStep->addDataRow({{"Val", Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", 90.0}});
    /*  9 */ pTestStep->addDataRow({{"Val", -Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 10 */ pTestStep->addDataRow({{"Val", -Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", -90.0}});
    /* 11 */ pTestStep->addDataRow({{"Val", Math::c_f135Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 12 */ pTestStep->addDataRow({{"Val", Math::c_f135Degrees_rad}, {"Normalize", false}, {"Angle_deg", 135.0}});
    /* 13 */ pTestStep->addDataRow({{"Val", -Math::c_f135Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 14 */ pTestStep->addDataRow({{"Val", -Math::c_f135Degrees_rad}, {"Normalize", false}, {"Angle_deg", -135.0}});
    /* 15 */ pTestStep->addDataRow({{"Val", Math::c_fPI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 16 */ pTestStep->addDataRow({{"Val", Math::c_fPI}, {"Normalize", false}, {"Angle_deg", 180.0}});
    /* 17 */ pTestStep->addDataRow({{"Val", -Math::c_fPI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 18 */ pTestStep->addDataRow({{"Val", -Math::c_fPI}, {"Normalize", false}, {"Angle_deg", -180.0}});
    /* 19 */ pTestStep->addDataRow({{"Val", Math::c_f225Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 20 */ pTestStep->addDataRow({{"Val", Math::c_f225Degrees_rad}, {"Normalize", false}, {"Angle_deg", 225.0}});
    /* 21 */ pTestStep->addDataRow({{"Val", -Math::c_f225Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 22 */ pTestStep->addDataRow({{"Val", -Math::c_f225Degrees_rad}, {"Normalize", false}, {"Angle_deg", -225.0}});
    /* 23 */ pTestStep->addDataRow({{"Val", Math::c_f270Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 24 */ pTestStep->addDataRow({{"Val", Math::c_f270Degrees_rad}, {"Normalize", false}, {"Angle_deg", 270.0}});
    /* 25 */ pTestStep->addDataRow({{"Val", -Math::c_f270Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /* 26 */ pTestStep->addDataRow({{"Val", -Math::c_f270Degrees_rad}, {"Normalize", false}, {"Angle_deg", -270.0}});
    /* 27 */ pTestStep->addDataRow({{"Val", Math::c_f315Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /* 28 */ pTestStep->addDataRow({{"Val", Math::c_f315Degrees_rad}, {"Normalize", false}, {"Angle_deg", 315.0}});
    /* 29 */ pTestStep->addDataRow({{"Val", -Math::c_f315Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /* 30 */ pTestStep->addDataRow({{"Val", -Math::c_f315Degrees_rad}, {"Normalize", false}, {"Angle_deg", -315.0}});
    /* 31 */ pTestStep->addDataRow({{"Val", Math::c_f2PI}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /* 32 */ pTestStep->addDataRow({{"Val", Math::c_f2PI}, {"Normalize", false}, {"Angle_deg", 360.0}});
    /* 33 */ pTestStep->addDataRow({{"Val", -Math::c_f2PI}, {"Normalize", true}, {"Angle_deg", 0.0}});
    /* 34 */ pTestStep->addDataRow({{"Val", -Math::c_f2PI}, {"Normalize", false}, {"Angle_deg", -360.0}});
    /* 35 */ pTestStep->addDataRow({{"Val", 9*Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 45.0}});
    /* 36 */ pTestStep->addDataRow({{"Val", 9*Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 405.0}});
    /* 37 */ pTestStep->addDataRow({{"Val", -9*Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 315.0}});
    /* 38 */ pTestStep->addDataRow({{"Val", -9*Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -405.0}});
    /* 39 */ pTestStep->addDataRow({{"Val", 5*Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 90.0}});
    /* 40 */ pTestStep->addDataRow({{"Val", 5*Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", 450.0}});
    /* 41 */ pTestStep->addDataRow({{"Val", -5*Math::c_f90Degrees_rad}, {"Normalize", true}, {"Angle_deg", 270.0}});
    /* 42 */ pTestStep->addDataRow({{"Val", -5*Math::c_f90Degrees_rad}, {"Normalize", false}, {"Angle_deg", -450.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", 11*Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 135.0}});
    /* 44 */ pTestStep->addDataRow({{"Val", 11*Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", 495.0}});
    /* 45 */ pTestStep->addDataRow({{"Val", -11*Math::c_f45Degrees_rad}, {"Normalize", true}, {"Angle_deg", 225.0}});
    /* 46 */ pTestStep->addDataRow({{"Val", -11*Math::c_f45Degrees_rad}, {"Normalize", false}, {"Angle_deg", -495.0}});
    /* 47 */ pTestStep->addDataRow({{"Val", Math::c_f3PI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 48 */ pTestStep->addDataRow({{"Val", Math::c_f3PI}, {"Normalize", false}, {"Angle_deg", 540.0}});
    /* 49 */ pTestStep->addDataRow({{"Val", -Math::c_f3PI}, {"Normalize", true}, {"Angle_deg", 180.0}});
    /* 50 */ pTestStep->addDataRow({{"Val", -Math::c_f3PI}, {"Normalize", false}, {"Angle_deg", -540.0}});

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "Deg2Rad",
        /* strOperation    */ "deg2Rad",
        /* pGrpParent      */ pGrpAngles,
        /* szDoTestStepFct */ SLOT(doTestStepConvertAngles(ZS::Test::CTestStep*)) );
    /*  1 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /*  2 */ pTestStep->addDataRow({{"Val", 0.0}, {"Normalize", false}, {"Angle_rad", 0.0}});
    /*  3 */ pTestStep->addDataRow({{"Val", 45.0}, {"Normalize", true}, {"Angle_rad", Math::c_f45Degrees_rad}});
    /*  4 */ pTestStep->addDataRow({{"Val", 45.0}, {"Normalize", false}, {"Angle_rad", Math::c_f45Degrees_rad}});
    /*  5 */ pTestStep->addDataRow({{"Val", -45.0}, {"Normalize", true}, {"Angle_rad", Math::c_f315Degrees_rad}});
    /*  6 */ pTestStep->addDataRow({{"Val", -45.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f45Degrees_rad}});
    /*  7 */ pTestStep->addDataRow({{"Val", 90.0}, {"Normalize", true}, {"Angle_rad", Math::c_f90Degrees_rad}});
    /*  8 */ pTestStep->addDataRow({{"Val", 90.0}, {"Normalize", false}, {"Angle_rad", Math::c_f90Degrees_rad}});
    /*  9 */ pTestStep->addDataRow({{"Val", -90.0}, {"Normalize", true}, {"Angle_rad", Math::c_f270Degrees_rad}});
    /* 10 */ pTestStep->addDataRow({{"Val", -90.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f90Degrees_rad}});
    /* 11 */ pTestStep->addDataRow({{"Val", 135.0}, {"Normalize", true}, {"Angle_rad", Math::c_f135Degrees_rad}});
    /* 12 */ pTestStep->addDataRow({{"Val", 135.0}, {"Normalize", false}, {"Angle_rad", Math::c_f135Degrees_rad}});
    /* 13 */ pTestStep->addDataRow({{"Val", -135.0}, {"Normalize", true}, {"Angle_rad", Math::c_f225Degrees_rad}});
    /* 14 */ pTestStep->addDataRow({{"Val", -135.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f135Degrees_rad}});
    /* 15 */ pTestStep->addDataRow({{"Val", 180.0}, {"Normalize", true}, {"Angle_rad", Math::c_fPI}});
    /* 16 */ pTestStep->addDataRow({{"Val", 180.0}, {"Normalize", false}, {"Angle_rad", Math::c_fPI}});
    /* 17 */ pTestStep->addDataRow({{"Val", -180.0}, {"Normalize", true}, {"Angle_rad", Math::c_fPI}});
    /* 18 */ pTestStep->addDataRow({{"Val", -180.0}, {"Normalize", false}, {"Angle_rad", -Math::c_fPI}});
    /* 19 */ pTestStep->addDataRow({{"Val", 225.0}, {"Normalize", true}, {"Angle_rad", Math::c_f225Degrees_rad}});
    /* 20 */ pTestStep->addDataRow({{"Val", 225.0}, {"Normalize", false}, {"Angle_rad", Math::c_f225Degrees_rad}});
    /* 21 */ pTestStep->addDataRow({{"Val", -225.0}, {"Normalize", true}, {"Angle_rad", Math::c_f135Degrees_rad}});
    /* 22 */ pTestStep->addDataRow({{"Val", -225.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f225Degrees_rad}});
    /* 23 */ pTestStep->addDataRow({{"Val", 270.0}, {"Normalize", true}, {"Angle_rad", Math::c_f270Degrees_rad}});
    /* 24 */ pTestStep->addDataRow({{"Val", 270.0}, {"Normalize", false}, {"Angle_rad", Math::c_f270Degrees_rad}});
    /* 25 */ pTestStep->addDataRow({{"Val", -270.0}, {"Normalize", true}, {"Angle_rad", Math::c_f90Degrees_rad}});
    /* 26 */ pTestStep->addDataRow({{"Val", -270.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f270Degrees_rad}});
    /* 27 */ pTestStep->addDataRow({{"Val", 315.0}, {"Normalize", true}, {"Angle_rad", Math::c_f315Degrees_rad}});
    /* 28 */ pTestStep->addDataRow({{"Val", 315.0}, {"Normalize", false}, {"Angle_rad", Math::c_f315Degrees_rad}});
    /* 29 */ pTestStep->addDataRow({{"Val", -315.0}, {"Normalize", true}, {"Angle_rad", Math::c_f45Degrees_rad}});
    /* 30 */ pTestStep->addDataRow({{"Val", -315.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f315Degrees_rad}});
    /* 31 */ pTestStep->addDataRow({{"Val", 360.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /* 32 */ pTestStep->addDataRow({{"Val", 360.0}, {"Normalize", false}, {"Angle_rad", Math::c_f2PI}});
    /* 33 */ pTestStep->addDataRow({{"Val", -360.0}, {"Normalize", true}, {"Angle_rad", 0.0}});
    /* 34 */ pTestStep->addDataRow({{"Val", -360.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f2PI}});
    /* 35 */ pTestStep->addDataRow({{"Val", 405.0}, {"Normalize", true}, {"Angle_rad", Math::c_f45Degrees_rad}});
    /* 36 */ pTestStep->addDataRow({{"Val", 405.0}, {"Normalize", false}, {"Angle_rad", 9.0*Math::c_fPI/4.0}});
    /* 37 */ pTestStep->addDataRow({{"Val", -405.0}, {"Normalize", true}, {"Angle_rad", Math::c_f315Degrees_rad}});
    /* 38 */ pTestStep->addDataRow({{"Val", -405.0}, {"Normalize", false}, {"Angle_rad", -9.0*Math::c_fPI/4.0}});
    /* 39 */ pTestStep->addDataRow({{"Val", 450.0}, {"Normalize", true}, {"Angle_rad", Math::c_f90Degrees_rad}});
    /* 40 */ pTestStep->addDataRow({{"Val", 450.0}, {"Normalize", false}, {"Angle_rad", 5.0*Math::c_fPI/2.0}});
    /* 41 */ pTestStep->addDataRow({{"Val", -450.0}, {"Normalize", true}, {"Angle_rad", Math::c_f270Degrees_rad}});
    /* 42 */ pTestStep->addDataRow({{"Val", -450.0}, {"Normalize", false}, {"Angle_rad", -5.0*Math::c_fPI/2.0}});
    /* 43 */ pTestStep->addDataRow({{"Val", 495.0}, {"Normalize", true}, {"Angle_rad", Math::c_f135Degrees_rad}});
    /* 44 */ pTestStep->addDataRow({{"Val", 495.0}, {"Normalize", false}, {"Angle_rad", 11.0*Math::c_fPI/4.0}});
    /* 45 */ pTestStep->addDataRow({{"Val", -495.0}, {"Normalize", true}, {"Angle_rad", Math::c_f225Degrees_rad}});
    /* 46 */ pTestStep->addDataRow({{"Val", -495.0}, {"Normalize", false}, {"Angle_rad", -11.0*Math::c_fPI/4.0}});
    /* 47 */ pTestStep->addDataRow({{"Val", 540.0}, {"Normalize", true}, {"Angle_rad", Math::c_fPI}});
    /* 48 */ pTestStep->addDataRow({{"Val", 540.0}, {"Normalize", false}, {"Angle_rad", Math::c_f3PI}});
    /* 49 */ pTestStep->addDataRow({{"Val", -540.0}, {"Normalize", true}, {"Angle_rad", Math::c_fPI}});
    /* 50 */ pTestStep->addDataRow({{"Val", -540.0}, {"Normalize", false}, {"Angle_rad", -Math::c_f3PI}});

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
    pTestStep->addDataRow({{"Val", 3452.78}, {"Result", 4}});
    pTestStep->addDataRow({{"Val", 0.00056}, {"Result", -4}});

    // Division Lines
    //---------------

    ZS::Test::CTestStepGroup* pGrpDivLines = new ZS::Test::CTestStepGroup(
        /* pTest      */ this,
        /* strName    */ "Division Lines",
        /* pGrpParent */ nullptr );

    pTestStep = new ZS::Test::CTestStep(
        /* pTest           */ this,
        /* strName         */ "CalculateDivLines4LinSpacing",
        /* strOperation    */ "calculateDivLines4LinSpacing",
        /* pGrpParent      */ pGrpDivLines,
        /* szDoTestStepFct */ SLOT(doTestStepCalculateDivLines4LinSpacing(ZS::Test::CTestStep*)) );
    pTestStep->addDataRow({
        {"ScaleMinVal", 0}, {"ScaleMaxVal", 0}, {"ScaleRangePix", 0},
        {"DivLineDistMinVal", 0}, {"DivLineDistMinPix", 0}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0}, {"DivLineDistFirstPix", 0}, {"DivLineDistVal", 0}, {"DivLineDistPix", 0},
        {"DivLinesCount", 0}});
    pTestStep->addDataRow({
        {"ScaleMinVal", 0}, {"ScaleMaxVal", 100.0}, {"ScaleRangePix", 1000},
        {"DivLineDistMinVal", 10.0}, {"DivLineDistMinPix", 50}, {"UseDivLineDistValDecimalFactor25", false},
        {"DivLineFirstVal", 0.0}, {"DivLineDistFirstPix", 0.0}, {"DivLineDistVal", 10.0}, {"DivLineDistPix", 100},
        {"DivLinesCount", 11}});

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

        if( strOperation.compare("rad2Deg") == 0)
        {
            fResult = Math::rad2Deg(fVal, bNormalize);
            mapResultValues["Angle_deg"] = QString::number(fResult);
        }
        else if( strOperation.compare("deg2Rad") == 0)
        {
            fResult = Math::deg2Rad(fVal, bNormalize);
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
            fResult = Math::round2Nearest(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Lower") == 0)
        {
            fResult = Math::round2Lower(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Upper") == 0)
        {
            fResult = Math::round2Upper(fVal, iTrailingDigits);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2Resolution") == 0)
        {
            fResult = Math::round2Resolution(fVal, fResolution);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2LowerDecade") == 0)
        {
            fResult = Math::round2LowerDecade(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2UpperDecade") == 0)
        {
            fResult = Math::round2UpperDecade(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2LowerIntMultOfTen") == 0)
        {
            fResult = Math::round2LowerIntMultOfTen(fVal);
            mapResultValues["Result"] = QString::number(fResult);
        }
        else if (strOperation.compare("round2UpperIntMultOfTen") == 0)
        {
            fResult = Math::round2UpperIntMultOfTen(fVal);
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
            std::tuple<double, double> minMax = Math::getAbsMinMax(arfVals);
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
            iDigit = Math::getFirstSignificantDigit(fVal);
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
void CTest::doTestStepCalculateDivLines4LinSpacing( ZS::Test::CTestStep* i_pTestStep )
//------------------------------------------------------------------------------
{
    QStringList strlstExpectedValues;
    QStringList strlstResultValues;

    for (int idxRow = 0; idxRow < i_pTestStep->getDataRowCount(); ++idxRow)
    {
        double  fScaleMinVal = 0.0;
        double  fScaleMaxVal = 0.0;
        int     iScaleRangePix = 0;
        double  fDivLineDistMinVal = 0.0;
        int     iDivLineDistMinPix = 0;
        bool    bUseDivLineDistValDecimalFactor25 = false;
        double  fDivLineFirstValExpected = 0.0;
        double  fDivLineDistFirstPixExpected = 0.0;
        double  fDivLineDistValExpected = 0.0;
        double  fDivLineDistPixExpected = 0.0;
        int     iDivLineCountExpected = 0;

        QMap<QString, QString> mapExpectedValues;
        QMap<QString, QString> mapResultValues;

        QHash<QString, QVariant> dataRow = i_pTestStep->getDataRow(idxRow);

        for (const QString& strKey : dataRow.keys())
        {
            if (strKey == "ScaleMinVal") {
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

        double fDivLineFirstValResult = 0.0;
        double fDivLineDistFirstPixResult = 0.0;
        double fDivLineDistValResult = 0.0;
        double fDivLineDistPixResult = 0.0;

        int iDivLineCountResult = Math::calculateDivLines4LinSpacing(
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

} // doTestStepCalculateDivLines4LinSpacing
