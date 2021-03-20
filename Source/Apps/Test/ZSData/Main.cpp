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

#include <QtCore/qglobal.h>

#include "App.h"

#include "ZSPhysSizes/Electricity/ZSPhysSizesElectricityDllMain.h"
#include "ZSPhysSizes/Kinematics/ZSPhysSizesKinematicsDllMain.h"
#include "ZSPhysVal/ZSPhysValDllMain.h"
#include "ZSSys/ZSSysVersion.h"

#include "ZSSys/ZSSysMemLeakDump.h"


/*******************************************************************************
Libraries depending on build configuration and used Qt version (Qt-Isar-Dll's)
*******************************************************************************/

#ifdef _WINDOWS

#ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#pragma comment( lib, QTMAINLIB )
#pragma comment( lib, QTCORELIB )
#pragma comment( lib, QTNETWORKLIB )
#pragma comment( lib, QTXMLLIB )
#pragma comment( lib, QTGUILIB )
#if QT_VERSION >= 0x050000 && QT_VERSION < 0x060000
#pragma comment( lib, QTWIDGETSLIB )
#endif

#endif // #ifdef USE_PRAGMA_COMMENT_LIB_INCLUDE_IN_MAIN_MODULES

#endif // #ifdef _WINDOWS


/*******************************************************************************
Entry point for the application.
*******************************************************************************/

//------------------------------------------------------------------------------
int main( int argc, char* argv[] )
//------------------------------------------------------------------------------
{
    /*  Possible command arguments:
        ---------------------------

        IniFileScope=<AppDir, System, User (default)>
        IniFileName="valid file name"

        TestStepsEnabled=<TestGroupName,..> with TestGroupName e.g. Db.Destroy
        TestStepsDisabled=<TestGroupName,..> with TestGroupName e.g. Db.Destroy

        AutoOpenDataSetDialogs

        Example:

            C:\Projekte\ZeusSoft\ZSQtLib\Bin\Win32>ZSAppTestData IniFileScope=AppDir AutoOpenDataSetDialogs TestStepsDisabled=Db.Destroy
    */

    int iAppResult = 0;

    ZS::PhysVal::createUnitsPool();

    ZS::PhysVal::Electricity::createPhysSizes();
    ZS::PhysVal::Kinematics::createPhysSizes();

    ZS::Apps::Test::Data::CApplication* pApp = new ZS::Apps::Test::Data::CApplication(
        /* argc                  */ argc,
        /* argv                  */ argv,
        /* strOrganizationName   */ "ZeusSoft",
        /* strOrganizationDomain */ "ZeusSoft.de",
        /* strAppName            */ "ZSAppTestData",
        /* strAppNameWindowTitle */ "ZSApp-Test-Data (" + ZS::System::c_strSysVersionNr + ")" );

    iAppResult = pApp->exec();

    delete pApp;
    pApp = nullptr;

    ZS::PhysVal::Electricity::deletePhysSizes();
    ZS::PhysVal::Kinematics::deletePhysSizes();

    ZS::PhysVal::deleteUnitsPool();

    #ifdef _WINDOWS
    #ifdef _DEBUG
    _CrtDoForAllClientObjects( ZS::dumpClientHook, nullptr );
    #endif
    #endif

    return iAppResult;

} // main
